#! /usr/bin/env python
import argparse, sys, os
import logging
logging.basicConfig(level=logging.DEBUG)
logger = logging.getLogger('read_gtf')

class GenomeTranscriptCoord(object):
    def __init__(self, gtf_table, sorted=True):
        '''gtf_table: a pandas DataFrame object created by read_table from a GTF file (sorted)
        '''
        exon = pd.DataFrame(gtf_table[gtf_table['feature'] == 'exon'])
        if not sorted:
            exon.sort_values(['chrom', 'start'], inplace=True)
        exon['start'] -= 1
        exon_starts = defaultdict(list)
        exon_ends = defaultdict(list)
        exon_sizes = defaultdict(list)
        strand = {}
        ind = exon.columns.get_loc('transcript_id')
        ind_strand = exon.columns.get_loc('strand')
        for row in exon.itertuples(index=False, name=None):
            exon_starts[row[ind]].append(row[3])
            exon_ends[row[ind]].append(row[4])
            exon_sizes[row[ind]].append(row[4] - row[3])
            strand[row[ind]] = row[ind_strand]
        self.exon_starts = exon_starts
        self.exon_ends = exon_ends
        self.exon_sizes = exon_sizes
        self.strand = strand

        print 'transcript_id: ENST00000540583.5'
        print exon_sizes['ENST00000540583.5']
        print exon_starts['ENST00000540583.5']
        print exon_ends['ENST00000540583.5']
        print self.spliced_transcript_length('ENST00000540583.5')

    def genome_to_spliced_transcript(self, transcript_id, pos):
        '''
            pos: 0-based position
        Returns:
            -1 if pos is not in the splices transcript
        '''
        exon_starts_t = self.exon_starts[transcript_id]
        exon_ends_t = self.exon_ends[transcript_id]
        exon_sizes_t = self.exon_sizes[transcript_id]
        strand_t = self.strand[transcript_id]
        for i in range(len(exon_starts_t)):
            if exon_starts_t[i] <= pos < exon_ends_t[i]:
                if i > 0:
                    pos_t = sum(exon_sizes_t[:i]) + pos - exon_starts_t[i]
                else:
                    pos_t = pos - exon_starts_t[i]
                if strand_t == '-':
                    pos_t = sum(exon_sizes_t) - pos_t - 1
                if transcript_id == 'ENST00000540583.5':
                    print '{} => {}'.format(pos, pos_t)
                return pos_t
        return -1

    def spliced_transcript_length(self, transcript_id):
        return sum(self.exon_sizes[transcript_id])

class GTFFile(object):
    def __init__(self, filename, cache=None):
        dtype = {'chrom': str, 'source': str, 'feature': str,
            'start': np.int64, 'end': np.int64, 'score': str,
            'strand': str, 'phase': str, 'attributes': str}
        if (cache is not None) and (os.path.isfile(cache)):
            logger.info('read GTF cache file: {}'.format(cache))
            self.data = pd.read_hdf(cache, 'data')
        else:
            logger.info('read GTF file: {}'.format(filename))
            self.data = pd.read_table(filename, header=None, comment='#', sep='\t', dtype=dtype,
                names=['chrom', 'source', 'feature', 'start', 'end', 'score', 'strand', 'phase', 'attributes'])
        if (cache is not None) and (not os.path.isfile(cache)):
            logger.info('write GTF cache file: {}'.format(cache))
            self.data.to_hdf(cache, 'data', mode='w', format='fixed')
        self.parse_attribute('transcript_id')

    def sort(self):
        self.data.sort_values(['chrom', 'start'], inplace=True)

    def write_table(self, outfile, feature, columns):
        data = self.data[self.data['feature'] == feature]
        outdata = {}
        for c in columns:
            if c in data.columns:
                outdata[c] = data[c].values
            elif c == 'spliced_transcript_length':
                gtcoord = GenomeTranscriptCoord(self.data, sorted=True)
                length = []
                for transcript_id in data['transcript_id']:
                    length.append(gtcoord.spliced_transcript_length(transcript_id))
                length = pd.Series(length, dtype='int64')
                outdata[c] = length
            else:
                a = data['attributes'].str.extract(c + ' "([^"]+)";', expand=False)
                a.name = c
                outdata[c] = a
        outdata = pd.DataFrame(outdata, columns=columns)
        outdata.to_csv(outfile, sep='\t', index=False, header=False, na_rep='nan')

    def parse_attribute(self, attribute):
        self.data[attribute] = self.data['attributes'].str.extract(attribute + ' "([^"]+)";', expand=False)

    def query(self, expression):
        self.data = self.data.query(expression)

    def attribute_equal(self, attribute, value):
        if attribute not in self.data.columns:
            self.parse_attribute(attribute)
        self.data = self.data[self.data[attribute] == value]

    def attribute_in(self, attribute, values):
        if attribute not in self.data.columns:
            self.parse_attribute(attribute)
        if len(values) == 1:
            self.data = self.data[self.data[attribute] == values[0]]
        else:
            self.data = self.data[self.data[attribute].isin(values)]

    def write_bed(self, outfile, feature=None):
        self.data['start'] = self.data['start'] - 1
        self.parse_attribute('transcript_id')

        if feature in ['3UTR', '5UTR']:
            outdata = pd.DataFrame(self.data[self.data['feature'] == 'UTR'])
            logger.info('get a list CDS start and end positions')
            cds_starts, cds_ends, cds_strand = self.get_block_list('CDS')
            for transcript_id in cds_starts:
                cds_starts[transcript_id] = min(cds_starts[transcript_id])
                cds_ends[transcript_id] = max(cds_ends[transcript_id])
            cds_starts = pd.Series(cds_starts, dtype='int64')
            cds_ends = pd.Series(cds_ends, dtype='int64')
            outdata.index = outdata['transcript_id']
            outdata['cds_start'] = cds_starts
            outdata['cds_end'] = cds_ends

            if feature == '5UTR':
                outdata = outdata.query('((strand == "+") and (end <= cds_start)) or ((strand == "-") and (start >= cds_end))')
            else:
                outdata = outdata.query('((strand == "+") and (start >= cds_end)) or ((strand == "-") and (end <= cds_start))')
        elif feature is not None:
            outdata = pd.DataFrame(self.data[self.data['feature'] == feature])
        else:
            outdata = self.data

        bed_names = ['chrom', 'start', 'end', 'name', 'score', 'strand']
        zeros = np.zeros(outdata.shape[0], dtype='int32')
        bed = dict(zip(bed_names, [outdata['chrom'], outdata['start'], outdata['end'],
            outdata['transcript_id'], zeros, outdata['strand']]))
        bed = pd.DataFrame(bed, columns=bed_names)
        print bed.head(3)
        bed.to_csv(outfile, index=False, header=False, na_rep='nan', sep='\t')

    def sample_gtf(self, outfile):
        self.parse_attribute('gene_id')
        gene_ids = self.data['gene_id'].unique()
        sampled_gene_ids = np.random.choice(gene_ids.values, 100, replace=False)
        self.data = self.data[self.data['gene_id'].isin(sampled_gene_ids)]
        self.data.to_csv(outfile, sep='\t', index=False, header=False)

    def write_transcript_bed(self, outfile, feature='exon'):
        """Get coordinates of a feature relative to the transcripts.
        """
        logger.info('create GenomeTranscriptCoord')
        gtcoord = GenomeTranscriptCoord(self.data, sorted=True)

        self.data['start'] -= 1
        self.parse_attribute('transcript_id')
        transcript = self.data[self.data['feature'] == 'transcript']
        bed_names = ['chrom', 'start', 'end', 'name', 'score', 'strand']

        # get start and end of CDS regions
        if feature in ['CDS', '3UTR', '5UTR']:
            logger.info('get a list CDS start and end positions')
            cds_starts, cds_ends, cds_strand = self.get_block_list('CDS')
            for transcript_id in cds_starts:
                cds_starts[transcript_id] = min(cds_starts[transcript_id])
                cds_ends[transcript_id] = max(cds_ends[transcript_id])

        logger.info('calculate start and end positions')

        if feature == 'CDS':
            start = []
            end = []
            transcript_ids =  cds_starts.keys()
            cds_start = []
            cds_end = []
            length = []
            for transcript_id in transcript_ids:
                start_t = gtcoord.genome_to_spliced_transcript(transcript_id, cds_starts[transcript_id])
                end_t = gtcoord.genome_to_spliced_transcript(transcript_id, cds_ends[transcript_id] - 1)
                if cds_strand[transcript_id] == '+':
                    start.append(start_t)
                    end.append(end_t + 1)
                else:
                    start.append(end_t)
                    end.append(start_t + 1)
                cds_start.append(cds_starts[transcript_id])
                cds_end.append(cds_ends[transcript_id])
                length.append(gtcoord.spliced_transcript_length(transcript_id))
                if transcript_id == 'ENST00000540583.5':
                    print 'strand: ', cds_strand[transcript_id]
                    print gtcoord.spliced_transcript_length(transcript_id)
            print len(transcript_ids), len(start), len(end), len(cds_start), len(cds_end), len(length)
            bed = pd.DataFrame({'chrom': transcript_ids, 'start': start, 'end': end,
                'cds_start': cds_start, 'cds_end': cds_end, 'length': length}, index=np.arange(len(transcript_ids)))
        elif feature == '5UTR':
            end = []
            transcript_ids = cds_starts.keys()
            for transcript_id in transcript_ids:
                if cds_strand[transcript_id] == '+':
                    end.append(gtcoord.genome_to_spliced_transcript(transcript_id, cds_starts[transcript_id]) + 1)
                else:
                    end.append(gtcoord.genome_to_spliced_transcript(transcript_id, cds_ends[transcript_id] - 1) + 1)
            bed = pd.DataFrame({'chrom': transcript_ids, 'end': end})
            bed['start'] = 0
        elif feature == '3UTR':
            start = []
            end = []
            transcript_ids = cds_starts.keys()
            for transcript_id in transcript_ids:
                if cds_strand[transcript_id] == '+':
                    start.append(gtcoord.genome_to_spliced_transcript(transcript_id, cds_ends[transcript_id] - 1))
                else:
                    start.append(gtcoord.genome_to_spliced_transcript(transcript_id, cds_starts[transcript_id]))
                end.append(gtcoord.spliced_transcript_length(transcript_id))
            bed = pd.DataFrame({'chrom': transcript_ids, 'start': start, 'end': end})
        elif feature == 'transcript':
            end = []
            for transcript_id in transcript['transcript_id']:
                end.append(gtcoord.spliced_transcript_length(transcript_id))
            bed = pd.DataFrame({'chrom': transcript['transcript_id'], 'end': end})
            bed['start'] = 0
        bed['score'] = 0
        bed['name'] = bed['chrom']
        bed['strand'] = '+'

        logger.info('filter bed')
        bed = bed.ix[bed['end'] > bed['start'], bed_names]
        print bed.head(3)
        bed.to_csv(outfile, index=False, header=False, na_rep='nan', sep='\t')

    def get_block_list(self, feature):
        block = pd.DataFrame(self.data[self.data['feature'] == feature])
        block_starts = defaultdict(list)
        block_ends = defaultdict(list)
        block_strand = {}
        ind = block.columns.get_loc('transcript_id')
        ind_strand = block.columns.get_loc('strand')
        for row in block.itertuples(index=False, name=None):
            block_starts[row[ind]].append(row[3])
            block_ends[row[ind]].append(row[4])
            block_strand[row[ind]] = row[ind_strand]
        return block_starts, block_ends, block_strand

    def write_bed12(self, outfile, feature):
        # BED uses 0-based index
        self.data['start'] = self.data['start'] - 1
        self.parse_attribute('transcript_id')

        transcript = self.data[self.data['feature'] == 'transcript']

        if feature in ('3UTR', '5UTR'):
            block = pd.DataFrame(self.data[self.data['feature'] == 'UTR'])
            # get start and end position of each CDS
            cds = self.data[self.data['feature'] == 'CDS']
            cds_starts, cds_ends, cds_strand = self.get_block_list('CDS')

            block['cds_start'] = pd.Series(cds_starts)[block['transcript_id']].values
            block['cds_end'] = pd.Series(cds_ends)[block['transcript_id']].values
            # separate 5'UTR and 3'UTR
            if feature == '5UTR':
                block = block.query('((end <= cds_start) and (strand == "+")) or ((start >= cds_end) and (strand == "-"))')
            elif feature == '3UTR':
                block = block.query('((end <= cds_start) and (strand == "-")) or ((start >= cds_end) and (strand == "+"))')
        elif feature == 'intron':
            exon_starts, exon_ends = self.get_block_list('exon')
            ind = transcript.columns.get_loc('transcript_id')
            blockCount = []
            blockStarts = []
            blockSizes = []
            for row in transcript.itertuples(index=False, name=None):
                exon_starts_t = exon_starts[row[ind]]
                exon_ends_t = exon_ends[row[ind]]
                exon_count = len(exon_starts_t)
                intron_starts = []
                intron_sizes = []
                for i in range(exon_count - 1):
                    intron_starts.append(str(exon_ends_t[i]))
                    intron_sizes.append(str(exon_starts_t[i + 1] - exon_ends_t[i]))
                blockCount.append(exon_count - 1)
                blockStarts.append(','.join(intron_starts))
                blockSizes.append(','.join(intron_sizes))
        elif feature in ('CDS', 'exon'):
            block = pd.DataFrame(self.data[self.data['feature'] == feature])
        else:
            raise ValueError('unknown feature: {}'.format(feature))
        # get list of blocks (e.g. exon, CDS, UTR) in tuples (start, end)
        block_starts = defaultdict(list)
        block_ends = defaultdict(list)
        ind = block.columns.get_loc('transcript_id')
        for row in block.itertuples(index=False, name=None):
            block_starts[row[ind]].append(row[3])
            block_ends[row[ind]].append(row[4])

        # Build BED12 columns
        if feature != 'intron':
            blockCount = []
            blockStarts = []
            blockSizes = []
            ind = transcript.columns.get_loc('transcript_id')
            for row in transcript.itertuples(index=False, name=None):
                block_starts_t = block_starts[row[ind]]
                block_ends_t = block_ends[row[ind]]
                blockCount.append(len(block_starts_t))
                blockStarts.append(','.join(map(lambda x: str(x - row[3]), block_starts_t)))
                blockSizes.append(','.join(map(lambda a, b: str(a - b), block_ends_t, block_starts_t)))

        bed_names = ['chrom', 'start', 'end', 'name', 'score', 'strand',
            'thickStart', 'thickEnd', 'itemRgb', 'blockCount', 'blockSizes', 'blockStarts']
        zeros = np.zeros(transcript.shape[0], dtype='int32')
        bed = dict(zip(bed_names, [transcript['chrom'], transcript['start'], transcript['end'],
            transcript['transcript_id'], zeros, transcript['strand'],
            zeros, zeros, zeros,
            blockCount, blockSizes, blockStarts]))
        bed = pd.DataFrame(bed, columns=bed_names)
        bed = bed.query('blockCount > 0')
        print bed.head()
        bed.to_csv(outfile, index=False, header=False, na_rep='nan', sep='\t')

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Extract information from GTF files')
    parser.add_argument('-i', '--gtf-file', type=str, required=False, default='-')
    parser.add_argument('--transcript-types', type=str, required=False)
    parser.add_argument('-o', '--outfile', type=str, required=False, default='-')
    parser.add_argument('--sorted', action='store_true', required=False)
    parser.add_argument('--format', type=str, choices=('table', 'bed', 'transcript_bed', 'bed12', 'sample_gtf'), required=True)
    parser.add_argument('--feature', type=str, required=True)
    parser.add_argument('--columns', type=str, default='')
    parser.add_argument('--name', type=str, default='transcript_id',
        help='used as the 4th column for BED format')
    parser.add_argument('--gtf-cache', type=str, help='make a hd5 cache')
    args = parser.parse_args()

    import pandas as pd
    import numpy as np
    from collections import defaultdict

    gtf_file = open(args.gtf_file, 'r') if args.gtf_file != '-' else sys.stdin
    gtf = GTFFile(gtf_file, cache=args.gtf_cache)
    if not args.sorted:
        logger.info('sort gtf file')
        gtf.sort()
    outfile = open(args.outfile, 'w') if args.outfile != '-' else sys.stdout
    if args.transcript_types:
        logger.info('filter transcript_type: {}'.format(args.transcript_types))
        gtf.attribute_in('transcript_type', args.transcript_types.split(','))
    if args.format == 'table':
        logger.info('columns: {}'.format(args.columns))
        logger.info('write table to {}'.format(args.outfile))
        gtf.write_table(outfile, args.feature, args.columns.split(','))
    elif args.format == 'bed12':
        logger.info('write bed12 to {}'.format(args.outfile))
        gtf.write_bed12(outfile, args.feature)
    elif args.format == 'bed':
        logger.info('write bed to {}'.format(args.outfile))
        gtf.write_bed(outfile, args.feature)
    elif args.format == 'transcript_bed':
        logger.info('write transcript bed to {}'.format(args.outfile))
        gtf.write_transcript_bed(outfile, args.feature)
    elif args.format == 'sample_gtf':
        gtf.sample_gtf(outfile, 100)

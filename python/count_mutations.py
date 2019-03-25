#! /usr/bin/env python
import argparse
import sys
import os
import logging
logging.basicConfig(level=logging.INFO, format='[%(asctime)s] [%(levelname)s] %(name)s: %(message)s')

def open_file_or_stdout(filename):
    if filename == '-':
        return sys.stdout
    else:
        return open(filename, 'w')

def open_file_or_stdin(filename):
    if filename == '-':
        return sys.stdin
    else:
        return open(filename, 'r')

def append_extra_line(f):
    """Yield an empty line after the last line in the file
    """
    for line in f:
        yield line
    yield ''

def read_fasta(filename):
    with open(filename, 'r') as f:
        name = None
        seq = ''
        for line in append_extra_line(f):
            if line.startswith('>') or (len(line) == 0):
                if (len(seq) > 0) and (name is not None):
                    yield (name, seq)
                if line.startswith('>'):
                    name = line.strip()[1:].split()[0]
                    seq = ''
            else:
                if name is None:
                    raise ValueError('the first line does not start with ">"')
                seq += line.strip()

def test(args):
    import pysam
    logger.info('read input sam file: ' + args.input_file)
    fin = open_file_or_stdin(args.input_file)
    sam = pysam.AlignmentFile(fin)
    n_forward = 0
    n_reverse = 0
    n_cigars = [0]*10
    for read in sam:
        if read.is_reverse:
            n_reverse += 1
        else:
            n_forward += 1
        if read.cigartuples:
            for op, l in read.cigartuples:
                n_cigars[op] += 1
    cigar_ops = 'MIDNSHP=XB'
    print('number of cigar operations')
    for i in range(10):
        print('{}: {}'.format(cigar_ops[i], n_cigars[i]))
    print('number of forward reads: {}, reverse reads: {}'.format(n_forward, n_reverse))

def count_mutations(args):
    import pysam
    
    logger.info('read input sam file: ' + args.input_file)
    sam = pysam.AlignmentFile(args.input_file, 'rb')
    logger.info('read reference file: ' + args.reference_file)
    reference = read_fasta(args.reference_file)

    #fin = open_file_or_stdin(args.input_file)
    logger.info('open output file: ' + args.output_file)
    fout = open_file_or_stdout(args.output_file)
    pileup_args = ['min_mapping_quality', 'max_depth', 'stepper', 'ignore_overlaps', 
        'flag_filter', 'flag_require', 'ignore_orphans', 'min_base_quality', 
        'adjust_capq_threshold', 'compute_baq', 'truncate']
    args_dict = vars(args)
    kwargs = {}
    for arg in pileup_args:
        if args_dict[arg] is not None:
            kwargs[arg] = args_dict[arg]
    header = ['ref_name', 'ref_length', 'ref_base', 'position', 'depth'] 
    pileup_fields = ['num_reads'] + list('ATCGN') + ['DEL', 'INS', 'REFSKIP']
    header += pileup_fields
    fout.write('\t'.join(header) + '\n')
    for ref_name, ref_seq in reference:
        ref_seq = ref_seq.upper()
        length = len(ref_seq)
        for pileup_column in sam.pileup(ref_name, 0, length, **kwargs):
            record = [ref_name, length, ref_seq[pileup_column.reference_pos], 
                pileup_column.reference_pos + 1, pileup_column.nsegments]
            c = {a:0 for a in pileup_fields}
            for pileup_read in pileup_column.pileups:
                c['num_reads'] += 1
                if pileup_read.is_del:
                    c['DEL'] += 1
                elif pileup_read.indel > 0:
                    c['INS'] += 1
                else:
                    if not pileup_read.is_refskip:
                        a = pileup_read.alignment.query_sequence[pileup_read.query_position]
                        c[a] += 1
                    else:
                        c['REFSKIP'] += 1

            record += [c[a] for a in pileup_fields]
            fout.write('\t'.join([str(a) for a in record]) + '\n')
    fout.close()

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Misc toys for SAM/BAM files')
    parser.add_argument('--input-file', '-i', type=str, required=True, help='input BAM file')
    parser.add_argument('--output-file', '-o', type=str, default='-', help='output file')
    parser.add_argument('--reference-file', '-r', type=str, required=True, help='reference FASTA file')
    # help messages are copied from https://github.com/pysam-developers/pysam/blob/master/pysam/libcalignmentfile.pyx
    parser.add_argument('--max-depth', type=int,
        help='''Maximum read depth permitted. The default limit is '8000'.''')
    parser.add_argument('--min-mapping-quality', type=int,
        help='''only use reads above a minimum mapping quality. The default is 0.''')
    parser.add_argument('--min-base-quality', type=int,
        help='''Minimum base quality. Bases below the minimum quality will
           not be output.''')
    parser.add_argument('--flag-require', type=int,
        help='''only use reads where certain flags are set. The default is 0.''')
    parser.add_argument('--flag-filter', type=int,
        help='''ignore reads where any of the bits in the flag are set. The default is
           BAM_FUNMAP | BAM_FSECONDARY | BAM_FQCFAIL | BAM_FDUP.''')
    parser.add_argument('--stepper', type=str,
        help='''The stepper controls how the iterator advances.
           Possible options for the stepper are
           ``all``
              skip reads in which any of the following flags are set:
              BAM_FUNMAP, BAM_FSECONDARY, BAM_FQCFAIL, BAM_FDUP
           ``nofilter``
              uses every single read turning off any filtering.
           ``samtools``
              same filter and read processing as in :term:`csamtools`
              pileup. For full compatibility, this requires a
              'fastafile' to be given. The following options all pertain
              to filtering of the ``samtools`` stepper.''')
    parser.add_argument('--adjust-capq-threshold', type=int,
        help='''adjust mapping quality. The default is 0 for no
           adjustment. The recommended value for adjustment is 50.''')
    parser.add_argument('--ignore-orphans', type=bool,
        help='''ignore orphans (paired reads that are not in a proper pair).
            The default is to ignore orphans.''')
    parser.add_argument('--compute-baq', type=bool,
        help='''re-alignment computing per-Base Alignment Qualities (BAQ). The
           default is to do re-alignment. Realignment requires a reference
           sequence. If none is present, no realignment will be performed.''')
    parser.add_argument('--ignore-overlaps', type=bool,
        help='''If set to True, detect if read pairs overlap and only take
           the higher quality base. This is the default.''')
    parser.add_argument('--truncate', type=bool,
        help='''By default, the samtools pileup engine outputs all reads
           overlapping a region. If truncate is True and a region is
           given, only columns in the exact region specificied are
           returned.''')

    args = parser.parse_args()
    logger = logging.getLogger('count_mutations')

    count_mutations(args)

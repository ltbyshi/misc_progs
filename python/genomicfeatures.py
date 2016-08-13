import re
import json
import pandas as pd
import numpy as np
import sqlite3
from pybedtools import BedTool

class GTFReader:
    features_basic = ('seqname', 'source', 'feature', 'start', 'end', 'score', 'strand', 'frame')
    
    def __init__(self, gtffile, feature):
        self.feature = feature
        self.f = open(gtffile, 'r')
    
    def __iter__(self):
        return self
    
    def next(self):
        valid = False
        line = None
        tokens = None
        while not valid:
            line = self.f.next()
            if line.startswith('#'):
                valid = False
                continue
            tokens = line.strip().split('\t')
            if len(tokens) < 8:
                valid = False
                continue
            if tokens[2] != self.feature:
                continue
            valid = True
            
        features = {name:value for name, value in zip(GTFReader.features_basic, tokens)}
        for attr in [attr.strip() for attr in tokens[8].split(';') if attr]:
            pos = attr.find(' ')
            if pos >= 0:
              name = attr[:pos]
              value = attr[pos:].strip('" ')
              features[name] = value
        for i in xrange(len(tokens[8:])/3):
            features[tokens[i*3+8]] = tokens[i*3+9]
        return features
    
class SQLiteBuffer:
    def __init__(self, conn, sql, bufsize=100000):
        self.conn = conn
        self.cursor = conn.cursor()
        self.sql = sql
        self.bufsize = bufsize
        self.records = []
        
    def execute(self, record):
        self.records.append(record)
        if len(self.records) > self.bufsize:
            self.flush()
            
    def flush(self):
        if self.records:
            self.cursor.executemany(self.sql, self.records)
            self.conn.commit()
            self.records = []
        
    def close(self):
        self.cursor.close()
        
def createdb_fromgtf(gtffile, dbfile, bufsize=500000):
    # gene_id => id
    genes = {}
    transcripts = {}
    exons = {}
    
    conn = sqlite3.connect(dbfile)
    cursor = conn.cursor()
    cursor.execute('PRAGMA journal_mode = OFF')
    conn.commit()
    # gene
    cursor.execute('DROP TABLE IF EXISTS Gene')
    cursor.execute('''CREATE TABLE Gene (
      id INTEGER PRIMARY KEY AUTOINCREMENT,
      seqname TEXT, start INTEGER, end INTEGER, length INTEGER,
      strand TEXT, 
      gene_id TEXT, gene_type TEXT, gene_name TEXT,
      gene_status TEXT)''')
    conn.commit()
    seq = 0
    sqlbuffer = SQLiteBuffer(conn, 'INSERT INTO Gene VALUES (%s)'%(','.join(itertools.repeat('?', 10))))
    for features in GTFReader(gtffile, 'gene'):
        seq += 1
        genes[features['gene_id']] = seq
        sqlbuffer.execute((seq, features['seqname'], features['start'], features['end'],
                int(features['end']) - int(features['start']),
                features['strand'], features['gene_id'].split('.')[0], 
                features['gene_type'], features['gene_name'], features['gene_status']))
    sqlbuffer.flush()
    sqlbuffer.close()
    cursor.execute('CREATE INDEX GeneIndex ON Gene(gene_id, gene_name)')
    conn.commit()
    
class GenomicFeature:
    attr_pat = re.compile(r'^(\w+)\s*"(.*)"$', re.I)
    def __init__(self):
        pass
    
    def __str__(self):
        if self.attrs:
            attr_str = '; '.join(['%s "%s"'%item for item in self.attrs.iteritems()]) + ';'
        return '\t'.join([self.seqname, self.source, self.feature, 
                    str(self.start), str(self.end), self.score,
                    self.strand, self.frame, attr_str])
    
    def dump_table(self, all_attrs):
        attr_str_list = []
        for key in all_attrs:
            if self.attrs.has_key(key):
                attr_str_list.append(self.attrs[key])
            else:
                attr_str_list.append('NA')
        return '\t'.join([self.seqname, self.source, self.feature, 
                    str(self.start), str(self.end), self.score,
                    self.strand, self.frame] + attr_str_list)
    
    @staticmethod
    def from_gff_line(line):
        fields = line.split('\t')
        f = GenomicFeature()
        f.seqname = fields[0]
        f.source = fields[1]
        f.feature = fields[2]
        f.start = int(fields[3])
        f.end = int(fields[4])
        f.score = fields[5]
        f.strand = fields[6]
        f.frame = fields[7]
        
        attrs = [attr.strip() for attr in fields[8].split(';') if attr]
        f.attrs = {}
        for attr in attrs:
            m = GenomicFeature.attr_pat.match(attr)
            if m:
                f.attrs[m.group(1)] = m.group(2)
        return f

def intersect(start1, end1, start2, end2):
    '''Returns the intersection between two intervals or
    returns None if no intersection is found
    '''
    if end2 < start1:
        return None
    if start2 > end1:
        return None
    if (start2 < start1):
        if start1 <= end2 <= end1:
            return (start1, end2)
        if end2 > end1:
            return (start1, end1)
    if (start1 <= start2 < end1):
        if start1 <= end2 <= end1:
            return (start2, end2)
        if end2 > end1:
            return (start2, end1)
    return None
    
class Transcript:
    def __init__(self, chrom='', start=0, length=0, strand='', exon_count=1,
              gene_id=None, transcript_id=None, **kwargs):
        self.gene_id = gene_id
        self.transcript_id = transcript_id
        self.chrom = chrom
        self.start = start
        self.length = length
        self.strand = strand
        self.exon_count = exon_count
        # rel_start, rel_end, geno_start, geno_end
        self.exon_int = np.zeros((exon_count, 4), dtype='int64')
        self.exon_length = np.zeros(exon_count, dtype='int64')
        
    def __repr__(self):
        return 'Chrom: %s, start: %d, length: %d, exons: %d'%(self.chrom, self.start, self.length, self.exon_count)
    
    def to_dict(self):
        d = {}
        d['gene_id'] = self.gene_id
        d['transcript_id'] = self.transcript_id
        d['chrom'] = self.chrom
        d['length'] = self.length
        d['strand'] = self.strand
        d['exon_count'] = self.exon_count
        d['exon_int'] = list(self.exon_int.reshape(4*self.exon_count))
        d['exon_length'] = list(self.exon_length)
        return d
    
    @staticmethod
    def from_dict(d):
        t = Transcript(**d)
        t.exon_length = np.array(d['exon_length'], dtype='int64')
        t.exon_int = np.array(d['exon_int'], dtype='int64').reshape((t.exon_count, 4))
        return t
    
    def relpos(self, pos):
        '''Convert genomic position to transcript position.
        '''
        for rel_start, rel_end, geno_start, geno_end in self.exon_int:
            if geno_start <= pos < geno_end:
                return (pos - geno_start + rel_start)
        return -1
    
    def genopos(self, pos):
        '''Convert transcript position to genomic position.
        '''
        for rel_start, rel_end, geno_start, geno_end in self.exon_int:
            if rel_start <= pos < rel_end:
                return (pos - rel_start + geno_start)
        return -1

    def relint(self, start, end):
        '''Convert genomic interval to transcript interval
        '''
        intervals = []
        for rel_start, rel_end, geno_start, geno_end in self.exon_int:
            region = intersect(start, end, geno_start, geno_end)
            if region:
                offset = rel_start - geno_start
                intervals.append((region[0] + offset, region[1] + offset))
        return intervals
    
    def genoint(self, start, end):
        '''Convert transcript interval to genomic interval
        '''
        intervals = []
        for rel_start, rel_end, geno_start, geno_end in self.exon_int:
            region = intersect(start, end, rel_start, rel_end)
            if region:
                offset = geno_start - rel_start
                intervals.append((region[0] + offset, region[1] + offset))
        return intervals
        
def ConstructTranscripts(exonAnnoFile):
    '''input: ExonAnnoFile (gend id) sorted by genomic coordinates
    col4: gene_id, col7: transcript_id
    returns: a dict of {gene_id: Transcript object}
    '''
    df_exons = BedTool(exonAnnoFile).to_dataframe()
    df_exons.rename(columns={'name': 'gene_id', 'thickStart': 'transcript_id'}, inplace=True)
    df_exons['length'] = df_exons['end'] - df_exons['start']
    # calculate transcript length
    df_transcripts_length = df_exons.groupby('gene_id', as_index=False)['length'].sum()
    df_transcripts_length.rename(columns={'length': 'transcript_length'}, inplace=True)
    # extract transcript start position
    df_transcripts_start = df_exons.groupby('gene_id', as_index=False)['start'].min()
    df_transcripts_start.rename(columns={'start': 'transcript_start'}, inplace=True)
    # merge transcript length and transcript start position into one data frame
    df_transcripts=pd.merge(pd.merge(df_exons, df_transcripts_start, on='gene_id'), 
                         df_transcripts_length, on='gene_id')
    # calculate start position of exons relative to the transcripts
    df_transcripts_rel_start = df_transcripts.groupby('gene_id', as_index=False)['length'].cumsum()
    df_transcripts['rel_start'] = df_transcripts_rel_start - df_transcripts['length']
    # create a dict of gene id: Transcript objects
    transcripts = {}
    for name, group in df_transcripts.groupby('gene_id'):
        firstrow = group.irow(0)
        gene_id = firstrow['gene_id']
        t = Transcript(chrom=firstrow['chrom'], start=firstrow['transcript_start'],
                  length=firstrow['transcript_length'], strand=firstrow['strand'], 
                  exon_count=len(group), gene_id = row['gene_id'], 
                  transcript_id=row['transcript_id'])
        i = 0
        for name, row in group.iterrows():
            t.exon_length[i] = row['length']
            t.exon_int[i][0] = row['rel_start']
            t.exon_int[i][1] = row['rel_start'] + row['length']
            t.exon_int[i][2] = row['start']
            t.exon_int[i][3] = row['end']
            i += 1
        
        transcripts[gene_id] = t
        
    return transcripts

def SaveTranscripts(transcripts, fname):
    objs = {}
    for name, t in transcripts.iteritems():
        objs[name] = t.to_dict()
    json.dump(objs, open(fname, 'w'))
    
def LoadTranscripts(fname):
    objs = json.load(open(fname, 'r'))
    transcripts = {}
    for name, d in objs.iteritems():
        transcripts[name] = Transcript.from_dict(d)
    return transcripts

class ChromScore(object):
    def __init(self, chrom_name, chrom_size, chunk_size, dtype):
        self.name = chrom_name
        self.chrom_size = chrom_size
        self.index_size = chrom_size/chunk_size + 1
        self.dtype = dtype
        self.index = np.fill(index_size, -1, dtype='int32')
        self.values = []
    
    def __setitem__(self, key, value):
        chunk = key/self.chunk_size
        if (0 <= chunk) & (chunk < self.index_size):
            i = key % self.chunk_size
            if self.index[i] < 0:
                # create a new chunk
                self.values.append(np.zeros(self.chunk_size, dtype=self.dtype))
                self.index[i] = len(self.values) - 1
            self.values[i] = value
        else:
            raise IndexError('ChromeScore index out of range (0, %d)'%self.chrom_size)
        
    def __getitem__(self, key, value):
        if (0 <= key) & (key < self.chrom_size):
            raise IndexError('ChromScore index out of range (0, %d)'%self.chrom_size)
        chunk = key/self.chunk_size
        i = key % self.chunk_size
        if self.index[i] < 0:
            # chunk not avaiable
            return np.nan
        else:
            return self.values[i]
        
        
class FeatureScore(object):
    def __init__(self, chrom_names, chrom_sizes, dtype, chunk_size=5000):
        self.dtype = dtype
        self.chrom_names = np.asarray(chrom_names)
        self.chrom_sizes = np.asarray(chrom_sizes, dtype='int64')
        self.chroms = {}
        for name, size, index_size in zip(chrom_names, chrom_sizes):
            self.chroms[name] = ChromScore(name, size, self.chunk_size, dtype)
            
    def save(self, fname):
        h5 = h5py.File(fname, 'w')
        h5.attrs['chunk_size'] = self.chunk_size
        scoregrp = h5.create_group('chrom')
        indexgrp = h5.create_group('index')
        h5.create_dataset('chrom_sizes', data=chrom_sizes)
        h5.create_dataset('chrom_names', data=chrom_names)
        for chrom in self.chroms:
            scoregrp.create_dataset(chrom.name,
                                    (len(chrom.values), chrom.chunk_size),
                                    data=np.concatenate(chrom.values, axis=1),
                                    chunk=(1, chrom.chunk_size))
            indexgrp.create_dataset(chrom.name, data=chrom.index)
        h5.close()
        
    def load(fname):
        h5 = h5py.File(fname, 'r')
        genome = FeatureScore(chrom_names=h5['chrom_names'][:],
                              chrom_sizes=h5['chrom_sizes'][:],
                              dtype=h5['/chrom/chr1'].dtype,
                              chunk_size=h5.attrs['chunk_size'])
        for name in genome.chrom_names:
            genome.chroms[name].index = h5['/index/%s'%name]
            genome.chroms[name].values = h5['/chrom/%s'%name]
        
        h5.close()
        

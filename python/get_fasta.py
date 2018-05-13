#! /usr/bin/env python
import argparse, sys, os
from collections import namedtuple
import string

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

def read_bed(filename):
    bed_names = ['chrom', 'start', 'end', 'name', 'score', 'strand',
        'thickStart', 'thickEnd', 'itemRgb', 'blockCount', 'blockSizes', 'blockStarts']
    with open(filename, 'r') as f:
        for line in f:
            c = line.strip().split('\t')
            c[1] = int(c[1])
            c[2] = int(c[2])
            yield c

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Get FASTA sequence from bed file')
    parser.add_argument('-fi', type=str, required=True, help='input FASTA file')
    parser.add_argument('-bed', type=str, required=True, help='input BED file')
    parser.add_argument('-format', type=str, required=False,
        default='{name}::{chrom}:{start}-{end}({strand})',
        help='format for the sequence names in the generated FASTA file')
    parser.add_argument('-type', type=str, required=False, default='dna')
    parser.add_argument('-name', action='store_true', required=False,
        help='Use the name field for the FASTA header')
    parser.add_argument('-s', dest='strandness', action='store_true',
        required=False, help='consider strandness')
    parser.add_argument('-o', '--outfile', type=str, required=False, default='-',
        help='output FASTA file')
    args = parser.parse_args()

    fasta = dict(read_fasta(args.fi))
    fout = open(args.outfile, 'w') if args.outfile != '-' else sys.stdout
    header_format = '>' + args.format + '\n'
    if args.type == 'dna':
        reverse_table = string.maketrans('ATCG', 'TAGC')
    elif args.type == 'rna':
        reverse_table = string.maketrans('AUCG', 'UAGC')
    else:
        raise ValueError('unknown sequence type: {}'.format(args.type))

    for c in read_bed(args.bed):
        if c[0] not in fasta:
            print >>sys.stderr, 'Warning: {} is not a valid name in the FASTA file'.format(c[0])
            continue
        fout.write(header_format.format(chrom=c[0],
            start=c[1], end=c[2], name=c[3], score=c[4], strand=c[5]))
        seq = fasta[c[0]][c[1]:c[2]].upper()
        if (c[5] == '+') or args.strandness:
            fout.write(seq)
        elif c[5] == '-':
            fout.write(string.translate(seq, reverse_table)[::-1])
        fout.write('\n')
    fout.close()

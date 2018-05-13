#! /usr/bin/env python
import argparse, sys, os

class IndexedFastaReader(object):
    def __init__(self, filename, index_file=None):
        self.filename = filename
        self.index_file = filename + '.fai' if index_file is None else index_file
        self.read_index()
        self.fasta_f = open(self.filename, 'rb')

    def read_index(self):
        """Fasta index format spec: http://www.htslib.org/doc/faidx.html
        5 columns: name, length, offset, line_bases, line_width
        """
        self.index = {}
        with open(self.index_file, 'r') as f:
            for lineno, line in enumerate(f):
                c = line.strip().split()
                if len(c) != 5:
                    raise ValueError('expects 5 columns in the fasta index file but got {} at line '.format(len(c), lineno + 1))
                self.index[c[0]] = map(int, c[1:])

    def get(self, name, offset=0, length=None):
        if name not in self.index:
            return None
        else:
            ind = self.index[name]
            n_lines = ind[0]/ind[2]
            seq = ''
            self.fasta_f.seek(ind[1])
            for i in xrange(ind[0]/ind[2]):
                seq += self.fasta_f.read(ind[3])[:-1]
            remaining = ind[0] % ind[2]
            if remaining != 0:
                seq += self.fasta_f.read(remaining)
            return seq

    def __getitem__(self, name):
        return self.get(name)

    def __iter__(self):
        for name in self.names:
            yield self.get(name)

    def close(self):
        self.fasta_f.close()

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

if __name__ == '__main__':
    parser = argparse.ArgumentParser('Extract sequences from a FASTA file by sequence IDs')
    parser.add_argument('fasta_file', type=str)
    parser.add_argument('--ids', type=str,
        help='a comma-separated list of sequence IDs')
    parser.add_argument('-f', '--from-file', type=str, default='-',
        help='read sequence IDs from input file')
    parser.add_argument('--index', type=str,
        help='a FASTA index file'),
    parser.add_argument('-o', '--outfile', type=str, default='-',
        help='output file')
    args = parser.parse_args()

    if args.ids:
        ids = args.ids.split(',')
    else:
        if args.from_file == '-':
            fin = sys.stdin
        else:
            fin = open(args.from_file, 'r')
        ids = fin.read().strip().split()
        fin.close()
    if not args.index:
        args.index = args.fasta_file + '.fai'
    if args.outfile == '-':
        fout = sys.stdout
    else:
        fout = open(args.outfile, 'w')
    if os.path.isfile(args.index):
        reader = IndexedFastaReader(args.fasta_file)
        for id in ids:
            seq = reader.get(id)
            if seq is None:
                print >>sys.stderr, 'Warning: sequence ID "%s" cannot be found in the FASTA file'
            else:
                fout.write('>%s\n'%id)
                fout.write(seq)
                fout.write('\n')
    else:
        for id, seq in read_fasta(args.fasta_file):
            if id in ids:
                fout.write('>%s\n'%id)
                fout.write(seq)
                fout.write('\n')
    fout.close()

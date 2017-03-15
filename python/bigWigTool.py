#! /usr/bin/env python

import ctypes
from ctypes.util import find_library
import argparse
import sys, os
from collections import OrderedDict

"""Steps to build the jkweb shared library:
* Download the source from http://hgdownload.soe.ucsc.edu/admin/exe/userApps.src.tgz
* Enter the kent/src directory.
* Make sure that openssl-devel, libpng-devel, zlib-devel are installed
* Modify lib/makefile to add the following lines:
%.o: %.c
        ${CC} -fPIC ${COPT} ${CFLAGS} ${HG_DEFS} ${LOWELAB_DEFS} ${HG_WARN} ${HG_INC} ${XINC} -o $@ -c $<

$(MACHTYPE)/jkweb.so: $(O) $(MACHTYPE)
    gcc -fPIC -shared -o $(MACHTYPE)/libjkweb.so $(O) -Wl,-z,defs -L../htslib -lhts -lm -lz -lpng -lpthread -lcrypto -lssl
* Enter the htslib directory, run the following commands to generate libhts.a
CFLAGS="-fPIC -DUCSC_CRAM=0 -DNETFILE_HOOKS=0" ./configure"
make
* Enter the lib/ directory. Run
make x86_64/libjkweb.so
"""

class bbiInterval(ctypes.Structure):
    '''inc/common.h:
#define bits32 unsigned
----------------------
inc/bbiFile.h:
struct bbiInterval
/* Data on a single interval. */
    {
    struct bbiInterval *next;   /* Next in list. */
    bits32 start, end;          /* Position in chromosome, half open. */
    double val;             /* Value at that position. */
    };
    '''
    pass

bbiInterval._fields_ = [('next', ctypes.POINTER(bbiInterval)),
            ('start', ctypes.c_uint),
            ('end', ctypes.c_uint),
            ('val', ctypes.c_double)
            ]

class BigWigFile(object):
    def __init__(self, filename):
        self.bwf = bigWigFileOpen(bigwig_file)
        if not bwf:
            raise IOError('cannot open the bigwig file: ' + filename)
        self.lm = lmInit(0)

    def close(self):
        bbiFileClose(self.bwf)
        lmCleanup(ctypes.byref(self.lm))
        self.bwf = None

    def interval_query(self, chrom, start, end):
        """Returns a numpy array of all values in range chrom:start-end
        """
        interval = bigWigIntervalQuery(self.bwf, chrom, start, end, self.lm)
        values = np.full(end - start, np.nan, dtype='float64')
        while interval:
            interval = interval.contents
            values[(interval.start - start) : (interval.end - end)] = interval.val
            interval = interval.next
        return values

    def interval_query_blocked(self, chrom, start, end, block_starts, block_sizes):
        """Similar to get_values_from_bigwig except that only values in the blocks are fetched
        """
        n_blocks = len(block_starts)
        value_starts = np.cumsum([0] + block_sizes)[:-1]
        length = np.sum(block_sizes)
        values = np.full(length, np.nan, dtype='float64')

        n_covered = 0
        for i, block_start, block_size in zip(range(n_blocks), block_starts, block_sizes):
            block_start += start
            interval = bigWigIntervalQuery(self.bwf, chrom,
                block_start, block_start + block_size, self.lm)
            #print 'bigWigIntervalQuery(%s, %d, %d) => %s'%(chrom, block_start, block_start + block_size, repr(interval))
            while interval:
                interval = interval.contents
                values[(interval.start - block_start + value_starts[i]) :
                    (interval.end - block_start + value_starts[i])] = interval.val
                """
                print 'values[%d:%d] = bbiInterval(%d, %d) = %f'%(interval.start - block_start + value_starts[i],
                    interval.end - block_start + value_starts[i],
                    interval.start, interval.end, interval.val)
                """
                n_covered += (interval.end - interval.start)
                interval = interval.next
        #print 'value_starts = (%s), n_covered = %d'%(','.join(map(str, value_starts)), n_covered)
        return values

    def __del__(self):
        if self.bwf:
            self.close()

def init_jkweb(dll_path):
    '''inc/bigWig.h:
    struct bbiFile *bigWigFileOpen(char *fileName);
    /* Open up big wig file.   Free this up with bbiFileClose */
    #define bigWigFileClose(a) bbiFileClose(a)
    struct bbiInterval *bigWigIntervalQuery(struct bbiFile *bwf, char *chrom, bits32 start, bits32 end,
        struct lm *lm);
    ----------------------
    inc/bbiFile.h:
    void bbiFileClose(struct bbiFile **pBwf);
    /* Close down a big wig/big bed file. */
    ----------------------
    inc/localmem.h
    struct lm *lmInit(int blockSize);
    /* Create a local memory pool. Parameters are:
     *      blockSize - how much system memory to allocate at a time.  Can
     *                  pass in zero and a reasonable default will be used.
     */
    void lmCleanup(struct lm **pLm);
    /* Clean up a local memory pool. */
    '''
    # find the dynamic library named libjkweb.so
    jkweb = ctypes.cdll.LoadLibrary(dll_path)

    # import all functions in jkweb into the global namespace
    symbols = ('bigWigFileOpen', 'bbiFileClose', 'bigWigIntervalQuery',
        'lmInit', 'lmCleanup')
    for sym in symbols:
        print >>sys.stderr, 'Import function %s from %s'%(sym, dll_path)
        globals()[sym] = getattr(jkweb, sym)

    # set argument types and result types for imported functions
    bigWigFileOpen.argtypes = (ctypes.c_char_p,)
    bigWigFileOpen.restype = ctypes.c_void_p
    bbiFileClose.argtypes = (ctypes.c_void_p,)
    bbiFileClose.restype = None
    bigWigIntervalQuery.argtypes = (ctypes.c_void_p, ctypes.c_char_p, ctypes.c_uint, ctypes.c_uint, ctypes.c_void_p)
    bigWigIntervalQuery.restype = ctypes.POINTER(bbiInterval)
    lmInit.argtypes = (ctypes.c_int,)
    lmInit.restype = ctypes.c_void_p
    lmCleanup.argtypes = (ctypes.POINTER(ctypes.c_void_p),)
    lmCleanup.restype = None

def bigWigToValueList(bigwig_file, bed_file,
        strand=None, split=False):
    """Query the values in the input bigwig file.
    The intervals are specified in the input bed file.
    Arguments:
        strand: restrict to a specific strand in the input bed file
        split: use the blockStarts and blockSizes fields in the bed file
    Returns:
        a generator that yields pairs of (name, np.ndarray)
    """
    global np
    import numpy as np
    bwf = BigWigFile(bigwig_file)
    bedf = open(bed_file, 'r')
    lm = lmInit(0)
    for line in bedf:
        fields = line.strip().split('\t')
        if (strand is not None) and (strand != fields[5]):
            continue
        chrom = fields[0]
        start = int(fields[1])
        end = int(fields[2])
        name = fields[3]
        if split:
            block_starts = map(int, fields[11].split(','))
            block_sizes = map(int, fields[10].split(','))
            values = bwf.interval_query_blocked(chrom, start, end,
                block_starts, block_sizes)
        else:
            values = bwf.interval_query(chrom, start, end)
        yield (name, values)
    bwf.close()
    bedf.close()

def bigWigAverageOverBed(bigwig_file, bed_file, strand=None, split=False):
    for name, values in bigWigToValueList(bigwig_file, bed_file,
            strand=strand, split=split):
        values_covered = values[np.logical_not(np.isnan(values))]
        values0 = np.copy(values)
        values0[np.isnan(values0)] = 0
        # calculate statistics
        summary = OrderedDict()
        summary['name'] = name
        summary['size'] = values.shape[0]
        summary['covered'] = values_covered.shape[0]
        summary['sum'] = values_covered.sum()
        summary['mean0'] = values0.mean()
        summary['mean'] = values_covered.mean() if values_covered.shape[0] > 0 else 0
        print '\t'.join(map(str, summary.values()))

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='A tool for manipulating bigwig files')
    parser.add_argument('command', type=str,
        choices=('bigWigAverageOverBed',))
    parser.add_argument('--bigwig', type=str, required=True,
        help='bigwig file to operate on')
    parser.add_argument('--bed', type=str, required=True,
        help='input bed file specifying the genomic ranges to query')
    parser.add_argument('--split', action='store_true', required=False, default=False,
        help='use the blockStarts, blockSizes and blockCount field in the bed file')
    parser.add_argument('-S', '--strand', type=str, required=False,
        help='use a specific strand in the bed file')
    parser.add_argument('-o', '--outfile', type=str, required=False,
        help='the output file')
    parser.add_argument('-l', '--library', type=str, required=False,
        help='path of the dynamic library (libjkweb.so)')
    args = parser.parse_args()

    init_jkweb(args.library)
    if args.command == 'bigWigAverageOverBed':
        try:
            bigWigAverageOverBed(args.bigwig, args.bed,
                strand=args.strand, split=args.split)
        except IOError as e:
            if e.errno == 32:
                pass

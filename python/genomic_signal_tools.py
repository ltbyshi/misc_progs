#! /usr/bin/env python
import argparse, sys, os, errno
import logging
logging.basicConfig(level=logging.DEBUG, format='[%(asctime)s] [%(levelname)s] %(name)s: %(message)s')

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

def read_chrom_sizes(filename):
    chrom_sizes = {}
    with open(filename, 'r') as f:
        for line in f:
            chrom, size = line.strip().split('\t')
            size = int(size)
            chrom_sizes[chrom] = size
    return chrom_sizes

def bedgraph_to_hdf5(args):
    import h5py
    import numpy as np
    from tqdm import tqdm

    logger.info('read chrom sizes: ' + args.chrom_sizes_file)
    chrom_sizes = read_chrom_sizes(args.chrom_sizes_file)

    data = {chrom:np.zeros(size, dtype=np.float32) for chrom, size in chrom_sizes.items()}
    logger.info('read bedgraph file: ' + args.input_file)
    with open_file_or_stdin(args.input_file) as fin:
        for line in fin:
            c = line.strip().split('\t')
            data[c[0]][int(c[1]):int(c[2])] = float(c[3])
    logger.info('create output file: ' + args.output_file)
    with h5py.File(args.output_file, 'w') as f:
        g = f.create_group('data')
        chroms = sorted(data.keys())
        for chrom in tqdm(chroms, unit='chrom'):
            g.create_dataset(chrom, data=data[chrom], compression=True)
        f.create_dataset('chroms', data=np.asarray(chroms))
        f.create_dataset('chrom_sizes', data=np.asarray([chrom_sizes[chrom] for chrom in chroms]))

def bam_to_hdf5(args):
    import pysam
    import h5py
    import numpy as np
    from tqdm import tqdm

    logger.info('read chrom sizes: ' + args.chrom_sizes_file)
    chrom_sizes = read_chrom_sizes(args.chrom_sizes_file)

    offset = args.offset
    logger.info('read input BAM file: ' + args.input_file)
    data = {chrom:np.zeros(size, dtype=np.float32) for chrom, size in chrom_sizes.items()}
    fin = pysam.AlignmentFile(args.input_file, 'r')
    # count whole reads
    if offset is None:
        for r in fin:
            data_chrom = data.get(r.reference_name)
            if data_chrom is None:
                continue
            if r.mapping_quality == 255:
                data_chrom[r.reference_start:(r.reference_start + r.query_length)] += 1
    elif offset >= 0:
        for r in fin:
            if r.mapping_quality == 255:
                data_chrom = data.get(r.reference_name)
                if data_chrom is None:
                    continue
                if not r.is_reverse:
                    data_chrom[r.reference_start + offset] += 1
                else:
                    data_chrom[r.reference_start + r.reference_length - offset - 1] += 1
    fin.close()
    logger.info('create output file: ' + args.output_file)
    with h5py.File(args.output_file, 'w') as f:
        g = f.create_group('data')
        chroms = sorted(data.keys())
        for chrom in tqdm(chroms, unit='chrom'):
            g.create_dataset(chrom, data=data[chrom], compression=True)
        f.create_dataset('chroms', data=np.asarray(chroms))
        f.create_dataset('chrom_sizes', data=np.asarray([chrom_sizes[chrom] for chrom in chroms]))

def extract_from_bed12(args):
    import h5py
    import numpy as np
    import pandas as pd
    from tqdm import tqdm

    logger.info('read input HDF5 file: ' + args.input_file)
    with h5py.File(args.input_file, 'r') as f:
        data = {}
        for chrom in tqdm(f['data'].keys(), unit='chrom'):
            data[chrom] = f['data/' + chrom][:]
    logger.info('read bed file: ' + args.bed_file)
    bed = pd.read_table(args.bed_file, header=None)
    tx_data = {}
    for row in tqdm(bed.itertuples(index=False), total=bed.shape[0], unit='transcript'):
        block_sizes = np.asarray(row[-2].split(',')[:row[-3]], dtype=np.int32)
        block_starts = np.asarray(row[-1].split(',')[:row[-3]], dtype=np.int32) + row[1]
        block_ends = block_starts + block_sizes
        tx_data[row[3]] = np.concatenate([data[row[0]][start:end] for start, end in zip(block_starts, block_ends)])
        if row[5] == '-':
            tx_data[row[3]] = tx_data[row[3]][::-1]
    logger.info('create output file: ' + args.output_file)
    with h5py.File(args.output_file, 'w') as f:
        g = f.create_group('data')
        for tx_name in tqdm(bed.loc[:, 3], unit='transcript'):
            g.create_dataset(tx_name, data=tx_data[tx_name], compression=True)

if __name__ == '__main__':
    main_parser = argparse.ArgumentParser()
    subparsers = main_parser.add_subparsers(dest='command')

    '''
    genomic_signal_tools.py bedgraph_to_hdf5 -i input.bedGraph --chrom-sizes-file input.chrom_sizes -o output.h5
    genomic_signal_tools.py extract_from_bed12 -i input.h5 --bed-file input.bed -o output.h5
    '''
    parser = subparsers.add_parser('bedgraph_to_hdf5')
    parser.add_argument('--input-file', '-i', type=str, default='-',
        help='bedgraph file')
    parser.add_argument('--chrom-sizes-file', type=str, required=True,
        help='chrom sizes')
    parser.add_argument('--output-file', '-o', type=str, required=True,
        help='HDF5 file')

    parser = subparsers.add_parser('bam_to_hdf5')
    parser.add_argument('--input-file', '-i', type=str, default='-',
        help='BAM/SAM file')
    parser.add_argument('--chrom-sizes-file', type=str, required=True,
        help='chrom sizes')
    parser.add_argument('--output-file', '-o', type=str, required=True,
        help='HDF5 file')
    parser.add_argument('--offset', type=int,
        help='offset in each record')
    
    parser = subparsers.add_parser('extract_from_bed12')
    parser.add_argument('--input-file', '-i', type=str, required=True,
        help='HDF5 file')
    parser.add_argument('--bed-file', '-b', type=str, required=True,
        help='BED12 file')
    parser.add_argument('--output-file', '-o', type=str, required=True,
        help='HDF5 file')

    args = main_parser.parse_args()

    logger = logging.getLogger('genomic_signal_tools.' + args.command)

    if args.command == 'bedgraph_to_hdf5':
        bedgraph_to_hdf5(args)
    elif args.command == 'extract_from_bed12':
        extract_from_bed12(args)
    elif args.command == 'bam_to_hdf5':
        bam_to_hdf5(args)
    

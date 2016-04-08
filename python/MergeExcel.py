#! /usr/bin/env python

import pandas as pd
import argparse

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('infile1', type=str)
    parser.add_argument('infile2', type=str)
    parser.add_argument('column', type=str)
    parser.add_argument('outfile', type=str)
    args = parser.parse_args()

    column = u'交易买方'

    t1 = pd.read_excel(args.infile1)
    t2 = pd.read_excel(args.infile2)
    m = pd.merge(t1, t2, on=column)
    m.to_excel(args.outfile)



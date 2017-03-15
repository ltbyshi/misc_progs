#! /usr/bin/env python
import argparse
import sys, os

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Convert text tables to excel files')
    parser.add_argument('infiles', type=str, nargs='+', metavar='FILE',
            help='input text file')
    parser.add_argument('-o', '--outfile', type=str, required=True,
            help='output Excel file name')
    parser.add_argument('-d', '--delimiter', type=str, required=False,
            help='field separater in input files')
    parser.add_argument('--header', type=str, required=False, default='infer',
            help='row number to use as header')
    args = parser.parse_args()

    import pandas as pd
    from pandas.io.excel import ExcelWriter

    excel = ExcelWriter(args.outfile)
    for infile in args.infiles:
        t = pd.read_table(infile, header=args.header)
        t.to_excel(excel, sheet_name=os.path.basename(infile), index=False)
    excel.save()

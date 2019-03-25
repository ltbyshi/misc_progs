#! /usr/bin/env python
import sys
import os
import argparse
import numpy as np
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
from matplotlib.backends.backend_pdf import PdfPages
import seaborn as sns
sns.set()

plot_functions = {}
def plot_function(**types):
    def plot_function_gen(f):
        plot_functions[f.__name__] = (f, types)
        return f
    return plot_function_gen

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('name', type=str, choices=list(plot_functions.keys()))
    parser.add_argument('--option', '-O', type=str, action='append')
    parser.add_argument('--output-file', '-o', type=str, required=True)
    args = parser.parse_args()

    f, types = plot_functions.get(args.name)
    if f is None:
        raise ValueError('unknown plot name: {}. Available names: {}'.format(args.name, ', '.join(plot_functions.keys())))
    options = {}
    if args.option is not None:
        for option in args.option:
            c = option.split('=')
            if len(c) == 1:
                options[c] = True
            elif len(c) == 2:
                options[c[0]] = types[c[0]](c[1])
            else:
                raise ValueError('invalid option string: {}'.format(option))
    
    print('figure.dpi:', plt.rcParams['figure.dpi'])
    figs = f(**options)
    if isinstance(figs, list) or isinstance(figs, tuple):
        with PdfPages(args.output_file) as pdf:
            for fig in figs:
                pdf.savefig(fig)
    else:
        plt.savefig(args.output_file)


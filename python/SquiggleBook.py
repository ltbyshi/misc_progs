#! /usr/bin/env python
import argparse
import matplotlib
matplotlib.use('Agg')
import itertools
from tqdm import tqdm
from matplotlib.backends.backend_pdf import PdfPages

def squiggle(a, b, c, d):
    t = np.linspace(0, 2*np.pi, 1000)
    return np.sin(a*t)*np.cos(b*t), np.cos(c*t)*np.sin(d*t)

if __name__ == '__main__':
    parser = argparse.ArgumentParser('Plot squiggle lines')
    parser.add_argument('--size', type=int, default=2,
        help='size for each plot')
    parser.add_argument('-n', type=int, default=10)
    parser.add_argument('-o', '--outfile', type=str, required=True)
    args = parser.parse_args()

    import matplotlib.pyplot as plt
    import numpy as np
    with PdfPages(args.outfile) as pdf:
        for a, b in tqdm(itertools.product(range(args.n), range(args.n)), total=args.n**2):
            fig, axes = plt.subplots(args.n, args.n, figsize=(args.n*args.size, args.n*args.size))
            for c, d in itertools.product(range(args.n), range(args.n)):
                x, y = squiggle(a + 1, b + 1, c + 1, d + 1)
                axes[c, d].plot(x, y, linewidth=0.5)
                axes[c, d].set_title('(%d, %d, %d, %d)'%(a + 1, b + 1, c + 1, d + 1))
                axes[c, d].set_xticks([])
                axes[c, d].set_yticks([])
            fig.suptitle('a={}, b={}, page={}'.format(a + 1, b + 1, a*10 + b + 1))
            plt.tight_layout()
            pdf.savefig(fig)
            plt.close()
            plt.clf()

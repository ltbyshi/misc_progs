#! /usr/bin/env python
import argparse
import matplotlib
import itertools
matplotlib.use('Agg')

def squiggle(a, b, c, d):
    t = np.linspace(0, 2*np.pi, 200)
    return np.sin(a*t)*np.cos(b*t), np.cos(c*t)*np.sin(d*t)

space = {}
space[0] = lambda t: (2*t, t, t, t)
space[1] = lambda t: (2*t + 1, t, t, t)
space[2] = lambda t: (2*t + 2, 2*t, t, t)
space[3] = lambda t: (2*t + 1, 2*t, t, t)
space[3] = lambda t: (2*t - 1, 2*t, t, t)
space[4] = lambda t: (3*t + 1, 2*t, t, t)
space[5] = lambda t: (3*t + 2, 2*t, t, t)
space[6] = lambda t: (t + 1, t - 1, t, t)
space[7] = lambda t: (2*t, 2*t - 1, t, t)
space[8] = lambda t: (3*t, 3*t, t, t)
space[9] = lambda t: (3*t, 3*t + 1, t, t)
space[10] = lambda t: (3*t, 3*t + 2, t, t)
space[11] = lambda t: (2*t + 1, t, t, t)
space[12] = lambda t: (3*t, 2*t, 2*t, 2*t)
space[13] = lambda t: (3*t + 1, t, t, t)
space[14] = lambda t: (3*t + 2, t, t, t)
space[15] = lambda t: (3*t, t, t, t)

def generate_space(s=0, size=1):
    t = 1
    for i in range(size):
        r = space[s](t)
        while not all(map(lambda x: x > 0, r)):
            r = space[s](t)
            t += 1
        t += 1
        yield r

if __name__ == '__main__':
    parser = argparse.ArgumentParser('Plot squiggle lines')
    parser.add_argument('--size', type=int, default=2,
        help='size for each plot')
    parser.add_argument('-n', type=int, default=8)
    parser.add_argument('-a', type=int, default=1)
    parser.add_argument('-b', type=int, default=1)
    parser.add_argument('-c', type=int, default=1)
    parser.add_argument('-d', type=int, default=1)
    parser.add_argument('-s', type=int, default=0)
    parser.add_argument('-o', '--outfile', type=str, required=True)
    args = parser.parse_args()

    import matplotlib.pyplot as plt
    import numpy as np
    fig, axes = plt.subplots(args.n, args.n, figsize=(args.n*args.size, args.n*args.size))
    """
    for i, j in itertools.product(range(args.n), range(args.n)):
        i_image = i*args.n + j
        a, b, c, d = i_image + args.a, i_image + args.b, i_image + args.c, i_image + args.d
        x, y = squiggle(a, b, c, d)
        axes[i, j].plot(x, y)
        axes[i, j].set_title('(%d, %d, %d, %d)'%(a, b, c, d))
        axes[i, j].set_xticks([])
        axes[i, j].set_yticks([])
    """
    print 'generate space %s'%args.s
    generator = generate_space(s=args.s, size=args.n*args.n)
    for i, j in itertools.product(range(args.n), range(args.n)):
        a, b, c, d = next(generator)
        x, y = squiggle(a, b, c, d)
        axes[i, j].plot(x, y)
        axes[i, j].set_title('(%d, %d, %d, %d)'%(a, b, c, d))
        axes[i, j].set_xticks([])
        axes[i, j].set_yticks([])

    plt.savefig(args.outfile)

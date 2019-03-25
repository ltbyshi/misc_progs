#! /usr/bin/env python
import sys
import os
import argparse
import numpy as np
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
plt.rcParams['font.family'] = 'Arial'
# plt.rcParams['font.size'] = 12
# plt.rcParams['legend.fontsize'] = 12
from matplotlib.backends.backend_pdf import PdfPages
from matplotlib.patches import Ellipse, PathPatch, Circle
from matplotlib.lines import Line2D
from matplotlib.path import Path
from matplotlib.collections import EllipseCollection, LineCollection, CircleCollection
from matplotlib.animation import FuncAnimation
from matplotlib import patheffects
from matplotlib.patheffects import PathPatchEffect
from matplotlib.colors import to_rgba, to_rgba_array, ListedColormap, LinearSegmentedColormap, Colormap
from matplotlib import cm

plot_functions = {}
def plot_function(**types):
    def plot_function_gen(f):
        plot_functions[f.__name__] = (f, types)
        return f
    return plot_function_gen

@plot_function(N=int)
def ellipses(N=100):
    shapes = []
    for i in range(N):
        e = Ellipse(xy=np.random.rand(2)*10,
                    width=np.random.rand(),
                    height=np.random.rand(),
                    angle=np.random.rand()*360)
        shapes.append(e)
    fig, ax = plt.subplots(subplot_kw={'aspect': 'equal'}, figsize=(10, 10))
    for shape in shapes:
        ax.add_artist(shape)
        shape.set_clip_box(ax.bbox)
        shape.set_alpha(0.5)
        shape.set_facecolor(np.random.rand(3))
    ax.set_xlim(0, 10)
    ax.set_ylim(0, 10)
    return fig

@plot_function(text=str)
def path_effect(text='Sick Fermat'):
    fig = plt.figure(figsize=(8, 4))
    t = fig.text(0.0, 0.0, text, fontsize=30)
    t = fig.text(0.1, 0.2, text, fontsize=30)
    t.set_path_effects([PathPatchEffect(offset=(4, -4), hatch='xxxx', facecolor='gray'),
                    PathPatchEffect(edgecolor='white', linewidth=1.1, facecolor='black')])
    t = fig.text(0.1, 0.5, text[::-1], fontsize=30, color='white')
    t.set_path_effects([patheffects.Stroke(linewidth=3, foreground='black'),
                    patheffects.Normal()])
    t = fig.text(0.1, 0.7, text[::-1], fontsize=30)
    t.set_path_effects([patheffects.withSimplePatchShadow()])

    text_color = to_rgba('black')
    lines = [
        [[0, 1], [0.2, 0.2]],
        [[0, 1], [0.5, 0.5]],
        [[0, 1], [0.7, 0.7]],
        [[0.1, 0.1], [0, 1]]
    ]
    for line in lines:
        fig.lines.append(Line2D(line[0], line[1], transform=fig.transFigure, figure=fig, color=text_color, linewidth=0.5))


    return fig

@plot_function(n_rows=int, n_cols=int)
def figure_coordinates(n_rows=5, n_cols=5):
    import itertools

    fig = plt.figure(figsize=(10, 10))
    for i, j in itertools.product(range(n_rows), range(n_cols)):
        x, y = i/float(n_rows), j/float(n_cols)
        x_d, y_d = fig.transFigure.transform((x, y))
        fig.text(x + 0.01, y + 0.01, '({:.1f}, {:.1f})'.format(x, y), fontsize=10)
        fig.text(x + 0.01, y - 0.01, '({:.1f}, {:.1f})'.format(x_d, y_d), fontsize=10)
        fig.patches.append(Circle((x, y), 0.005, facecolor=to_rgba('blue'), figure=fig, transform=fig.transFigure))
    return fig

@plot_function(N=int)
def font_size(N=10):
    fig = plt.figure(figsize=(10, 10))
    # draw grid
    n_rows = 100
    n_cols = 100
    for i in range(1, n_rows):
        fig.lines.append(Line2D([float(i)/n_rows]*2, [0.0, 1.0], transform=fig.transFigure, figure=fig,
            color=to_rgba('gray'), linewidth=0.2))
    for i in range(1, n_cols):
        fig.lines.append(Line2D([0.0, 1.0], [float(i)/n_cols]*2, transform=fig.transFigure, figure=fig,
            color=to_rgba('gray'), linewidth=0.2))
    for i in range(N):
        fontsize = (i + 1)*10
        fig.text(0.0, float(i)/N, 'ABCDEF {}'.format(fontsize), fontsize=fontsize)
    return fig

@plot_function(N=int)
def scatter_circles(N=100):
    x = np.random.rand(N)
    y = np.random.rand(N)
    colors = np.random.rand(N)
    edgecolors = np.random.rand(N, 4)
    area = (50*np.random.rand(N))**2
    fig, ax = plt.subplots(subplot_kw={'aspect': 'equal'}, figsize=(10, 10))
    ax.scatter(x, y, s=area, c=colors, edgecolors=edgecolors, alpha=0.5)
    return fig

@plot_function()
def colormaps():
    listed_colormaps = {}
    linear_segmented_colormaps = {}
    for name, obj in cm.__dict__.items():
        if isinstance(obj, ListedColormap):
            listed_colormaps[name] = obj
        elif isinstance(obj, LinearSegmentedColormap) and not isinstance(obj, cm.ScalarMappable):
            linear_segmented_colormaps[name] = obj
    colormaps = list(listed_colormaps.items()) + list(linear_segmented_colormaps.items())
    n_plots_per_fig = 40
    n_plots = len(colormaps)
    n_figs = int(np.ceil(n_plots/float(n_plots_per_fig)))
    figs = []
    gradient = np.linspace(0, 1, 256)
    gradient = np.vstack([gradient, gradient])
    for name, colormap in colormaps:
        print(name, colormap)

    for i in range(n_figs):
        n_plots_page = min(n_plots_per_fig, n_plots - i*n_plots_per_fig)
        fig, axes = plt.subplots(n_plots_per_fig, figsize=(15, 15))
        fig.subplots_adjust(top=0.95, bottom=0.01, left=0.15, right=0.99)
        for j in range(n_plots_page):
            name, colormap = colormaps[i*n_plots_per_fig + j]
            print(name, colormap)
            if isinstance(colormap, ListedColormap):
                grad = np.arange(colormap.N)
                grad = np.vstack([grad, grad])
                axes[j].imshow(grad, aspect='auto', cmap=colormap)
            else:
                axes[j].imshow(gradient, aspect='auto', cmap=colormap)
            pos = list(axes[j].get_position().bounds)
            x_text = pos[0] - 0.01
            y_text = pos[1] + pos[3]/2
            fig.text(x_text, y_text, name, va='center', ha='right', fontsize=10)
        for j in range(n_plots_per_fig):
            axes[j].set_axis_off()
        figs.append(fig)
    return figs

@plot_function(Nmin=int, Nmax=int, Nstep=int)
def tiger_team_puzzle(Nmin=100, Nmax=300, Nstep=10):
    plt.rcParams.update({'figure.autolayout': False})
    figs = []
    for N in range(Nmin, Nmax + Nstep, Nstep):
        fig, ax = plt.subplots(figsize=(7, 7), subplot_kw={'aspect': 'equal'})
        ax.set_xlim(0, 1)
        ax.set_ylim(0, 1)
        ax.text(0.01, 0.01, 'This is the answer!', va='bottom', ha='left', size=30)
        ax.set_axis_off()
        k = np.arange(-N, N + 1)
        lines = np.empty((2*N + 1, 2, 2))
        lines[:, 0, 0] = k/(2*N)
        lines[:, 0, 1] = -k/(2*N)
        lines[:, 1, 0] = 1 + k/(2*N)
        lines[:, 1, 1] = 1 - k/(2*N)
        linecolors = np.tile([0.0, 0.0, 0.0, 1.0], 2*N + 1).reshape((2*N + 1, -1))
        ax.add_collection(LineCollection(lines,
                                        linewidths=np.full(2*N + 1, 0.5), 
                                        colors=linecolors))
        ax.set_title('N = {}'.format(N))
        ax.margins(x=0, y=0)
        plt.tight_layout()
        figs.append(fig)
    return figs

@plot_function()
def fancy_marker():
    codes = []
    verts = []
    rect = Path.unit_rectangle()
    for i, j in [(0.5, -0.5), (-0.5, 0.5), (-0.5, -1.5), (-1.5, -0.5)]:
        codes.append(rect.codes)
        verts.append(rect.vertices + np.asarray([i, j])[np.newaxis, :])
    codes = np.concatenate(codes)
    verts = np.concatenate(verts, axis=0)
    path = Path(verts, codes)

    fig, ax = plt.subplots(figsize=(10, 10))
    ax.plot(np.arange(10)**2, '--r', marker=path, markersize=30)
    return fig

@plot_function(nx=int, ny=int, nq=int, density=int)
def electric_field(nx=100, ny=100, nq=2*10, density=6):
    x = np.linspace(-2, 2, nx)
    y = np.linspace(-2, 2, ny)
    X, Y = np.meshgrid(x, y)
    # create charges
    q = np.ones(nq)
    q[::2] = -1
    #q *= np.random.uniform(size=nq)
    #q *= np.random.beta(0.2, 0.05, size=nq)
    i = np.arange(nq)
    charges_x = np.cos(2*np.pi*i/nq)
    charges_y = np.sin(2*np.pi*i/nq)
    charges_r = np.column_stack([charges_x, charges_y])
    charges_r = np.random.uniform(-2, 2, size=(nq, 2))
    # compute electric field vectors
    r = np.column_stack([X.ravel(), Y.ravel()])
    E_nom = q[np.newaxis, :, np.newaxis]*(r[:, np.newaxis, :] - charges_r[np.newaxis, :, :])
    E_den = np.power(np.sqrt(np.sum(np.square(r[:, np.newaxis, :] - charges_r[np.newaxis, :, :]), axis=2, keepdims=True)), 3)
    E = E_nom/E_den
    E = np.sum(E, axis=1)
    E = E.reshape(nx, ny, 2)
    E = np.clip(E, -20, 20)

    # create streamplot
    fig, ax = plt.subplots(subplot_kw={'aspect': 'equal'}, figsize=(15, 15))
    color = np.sqrt(np.sum(np.square(E), axis=-1))
    ax.streamplot(x, y, E[:, :, 0], E[:, :, 1], color=color, cmap=cm.inferno, 
                density=density, linewidth=0.5,  arrowsize=0.7)

    # plot circles for charges
    charge_colors = {True: '#aa0000', False: '#0000aa'}
    for i in range(nq):
        ax.add_artist(Circle(charges_r[i], 0.02, color=charge_colors[q[i] > 0]))
        
    ax.set_xlabel('x')
    ax.set_ylabel('y')
    return fig

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

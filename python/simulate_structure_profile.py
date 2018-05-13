#! /usr/bin/env python
import argparse, sys, os

if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('--input-file', '-i', type=str, required=True, help='input structure')
    args = parser.parse_args()

    import numpy as np
    with open(args.input_file, 'r') as f:
        structure = f.read().strip()
    structure = np.frombuffer(structure, dtype='S1')
    profile = np.zeros(structure.shape[0])
    profile[structure == '.'] = np.random.normal(0.8, 0.2, size=np.sum(structure == '.'))
    profile[structure != '.'] = np.random.normal(0.1, 0.2, size=np.sum(structure != '.'))

    sys.stdout.write('\n'.join(profile.astype('S')))
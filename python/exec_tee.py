#! /usr/bin/env python
import argparse, os, sys, errno
import subprocess

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='execute a program and dump stdin/stdout to file')
    parser.add_argument('--output-dir', '-o', type=str, default='exec_tee')
    parser.add_argument('command, type=str, required=True)
    args = parser.parse_args()

    p = subprocess.Popen([args.command] + args.arguments,
        stdin=subprocess.PIPE)
    buffer_size = 1024*512
    try:
        fout = open(os.path.join(args.output_dir + '')
        while True:
            data = sys.stdin.read(buffer_size)
            p.stdin.write(data)
            if len(data) < buffer_size:
                break
    except OSError as e:
        if e.errno == errno.EPIPE:
            pass
    
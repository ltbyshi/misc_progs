#! /usr/bin/env python

import argparse, os
import sys
import shutil

def MoveFile(source, dest, dry_run=True, move=False):
   
    parent_dir = os.path.dirname(dest)
    if not os.path.exists(parent_dir):
        print 'mkdir: {0}'.format(parent_dir)
        if not dry_run:
            os.makedirs(parent_dir)
    if move:
        print 'move {0} => {1}'.format(source, dest)
    else:
        print 'copy {0} => {1}'.format(source, dest)
    if dry_run:
        return

    if os.path.isdir(dest):
        if move:
            shutil.move(source, dest)
        else:
            shutil.copytree(source, dest)

def MergeDir(source, dest, dry_run=True, move=False):
    source_list = set(os.listdir(source))
    dest_list = set(os.listdir(dest))
    for path in (source_list - dest_list):
        MoveFile(os.path.join(source, path), 
                os.path.join(dest, path),
                dry_run, move)
    for path in (source_list & dest_list):
        source_full = os.path.join(source, path)
        dest_full = os.path.join(dest, path)
        if os.path.isdir(source_full) and os.path.isdir(dest_full):
            MergeDir(source_full, dest_full, dry_run, move)
        else:
            print 'Conflict: {0}'.format(source_full)


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('source', type=str)
    parser.add_argument('dest', type=str)
    parser.add_argument('--move', action='store_true', required=False, default=False)
    parser.add_argument('--dry-run', action='store_true', required=False, default=False)
    args = parser.parse_args()

    MergeDir(args.source, args.dest, args.dry_run, args.move)

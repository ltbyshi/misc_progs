#! /usr/bin/env python
import os
import sys
import stat

def lstat_link(path, max_depth=5):
    try:
        st = os.lstat(path)
        depth = 0
        prev_path = path
        while stat.S_ISLNK(st.st_mode) and (depth < max_depth):
            cur_path = os.readlink(prev_path)
            if not cur_path.startswith('/'):
                cur_path = path + '/' + cur_path
            st = os.lstat(cur_path)
            depth += 1
            prev_path = cur_path
        if not stat.S_ISLNK(st.st_mode):
            return st
    except FileNotFoundError:
        pass

def compare_dirs(path1, path2):
    is_diff = False
    stat1 = os.lstat(path1)
    #if stat1 is None:
    #    print('[notfound] ' + path1)
    stat2 = os.lstat(path2)
    #if stat2 is None:
    #    print('[notfound] ' + path2)
    #if (stat1 is None) or (stat2 is None):
    #   return True
    if stat1.st_mode != stat2.st_mode:
        print('[mode] {}: {} {}'.format(path1, stat1.st_mode, stat2.st_mode))
        is_diff = True
    elif not stat.S_ISDIR(stat1.st_mode):
        if stat1.st_size != stat2.st_size:
            print('[size] ' + path1)
            is_diff = True
        else:
            pass
    else:
        files1 = set(os.listdir(path1))
        files2 = set(os.listdir(path2))
        if files1 != files2:
            for filename in (files1 - files2):
                print('[file1] ' + os.path.join(path1, filename))
                is_diff = True
            for filename in (files2 - files1):
                print('[file2] ' + os.path.join(path2, filename))
                is_diff = True
        for filename in (files1 & files2):
            #print('[compare] {} {}'.format(os.path.join(path1, filename), os.path.join(path2, filename)))
            is_diff &= compare_dirs(os.path.join(path1, filename), os.path.join(path2, filename))
    return is_diff

    
if __name__ == '__main__':
    if len(sys.argv) < 3:
        print('Usage: {} dir1 dir2 '.format(sys.argv[0]))
        sys.exit(1)
    compare_dirs(sys.argv[1], sys.argv[2])

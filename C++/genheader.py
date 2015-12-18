#! /usr/bin/env python

import string
import sys
import os
import argparse
import random

prolog = string.Template('''#ifndef __${Guard}__
#define __${Guard}__

''')

epilog = string.Template('''
#endif /* ${FileName} */
''')

def GenRandomStrings(filename, n, length, varname='strings'):
    base = os.path.basename(filename)
    with open(filename, 'w') as f:
        f.write(prolog.substitute(Guard=base.upper().replace('.', '_')))
        f.write('const unsigned long n_%s = %d;\n'%(varname, n))
        f.write('const char* %s[%d] = {\n'%(varname, n))
        buf = bytearray(length);
        alphabet = [chr(i) for i in xrange(32, 127)]
        for a in ('?', '"', '\'', '\\', '%'):
            alphabet.remove(a)
        for i in xrange(n):
            for j in xrange(length):
                buf[j] = random.choice(alphabet)
            f.write('  "%s"'%str(buf))
            if i < (n - 1):
                f.write(',')
            f.write('\n')

        f.write('};\n')
        f.write(epilog.substitute(FileName=base))

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Generate a C header file with random strings')
    parser.add_argument('-l', '--length', type=int, required=False, default=80)
    parser.add_argument('-n', '--number', type=int, required=False, default=100)
    parser.add_argument('-s', '--varname', type=str, required=False, default='strings')
    parser.add_argument('filename', type=str)
    args = parser.parse_args()

    GenRandomStrings(args.filename, args.number, args.length, args.varname)


#! /usr/bin/env python
import re, os, sys

'''
cd fakechroot/
fgrep -r 'wrapper(' src | cut -d':' -f2 | sort > wrappers.txt
fgrep -r '#include' src | cut -d':' -f2 | sort | uniq > include.txt
'''

def parse_wrapper(line):
    # remove wrapper(...)
    line = line.strip()[8:-1]
    i = line.find(',')
    function = line[:i].strip()
    line = line[(i + 1):]
    i = line.find(',')
    return_type = line[:i].strip()
    line = line[(i + 1):]
    arguments = line.strip()

    return function, return_type, arguments

wrapper_template = '''
typedef {return_type} (*REAL_{function}){arguments};
static REAL_{function} real_{function} = NULL;

{return_type} {function}{arguments}
{
    if(!real_{function})
        real_{function} = (REAL_{function})dlsym(RTLD_NEXT, "{function}");
    fprintf(stderr, "[ld_preload] {function}\n");
    return real_{function}()
}
'''

if __name__ == '__main__':
    fout = sys.stdout
    includes = []
    with open('include.txt', 'r') as f:
        for line in f:
            if not line.startswith('#include'):
                continue
            c = line.strip().split()
            if c[1].startswith('<'):
                includes.append(c[1])
    
    for inc in includes:
        fout.write('#include {}\n'.format(inc))
    fout.write('\n')

    with open('wrappers.txt', 'r') as f:
        for line in f:
            if line.startswith('wrapper'):
                function, return_type, arguments = parse_wrapper(line)
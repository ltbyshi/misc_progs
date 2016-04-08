#! /usr/bin/env python

import sys
sys.setrecursionlimit(5000)
def NestedClass(f, prefix, depth):
    name = '%s%d'%(prefix, depth)
    name_next = '%s%d'%(prefix, depth - 1)
    f.write('class %s {\n'%name)
    f.write('public:\n')
    f.write('\t%s() { cout << "%s()" << endl; }\n'%(name, name))
    if depth > 1:
        NestedClass(f, prefix, depth - 1)
        f.write('\t%s m;\n'%name_next)
    f.write('};\n')

if __name__ == '__main__':
    depth = 2000
    f = sys.stdout
    f.write('#include <iostream>\n')
    f.write('using namespace std;\n\n')
    NestedClass(f, 'C', depth)
    f.write('int main()\n{\n')
    f.write('\tC%d c;\n'%depth)
    f.write('\treturn 0;\n}\n')

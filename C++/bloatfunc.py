#! /usr/bin/env python

from cStringIO import StringIO
import sys


def BloatFunc(nmembers=100, ntemplates=100):
    f = sys.stdout
    f.write('#include <iostream>\n')
    f.write('#include <typeinfo>\n')
    f.write('using namespace std;\n')
    f.write('template<int> struct Int2Type {};\n')
    f.write('template <typename T>\n')
    f.write('class A {\n')
    f.write('public:\n')
    f.write('template <typename U>\n')
    f.write('void Do() { cout << typeid(U).name() << endl; }\n')
    f.write('void DoAll() {\n')
    for i in xrange(nmembers):
        f.write('Do<Int2Type<%d> >();\n'%i)
    f.write('}\n')
    f.write('};\n')

    f.write('void DoAll() {\n')
    for i in xrange(ntemplates):
        f.write('A<Int2Type<%d> >().DoAll();\n'%i)
    f.write('}\n')

    f.write('int main() { DoAll(); return 0; }\n\n')
    
if __name__ == '__main__':
    BloatFunc(50, 50)


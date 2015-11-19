#! /usr/bin/env python

from string import Template
import os

if __name__ == '__main__':
    template = Template(open('interrupt.cpp.in', 'r').read())
    for i in xrange(256*256):
        IntCode = '0x%02x'%i
        print 'Interrupt ' + IntCode
        source = template.substitute(IntCode=IntCode)
        open('/tmp/interrupt.cpp', 'w').write(source)
        os.system('g++ -g -o /tmp/interrupt /tmp/interrupt.cpp')
        os.system('/tmp/interrupt')
        os.unlink('/tmp/interrupt')


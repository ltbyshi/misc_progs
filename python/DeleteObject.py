#! /usr/bin/env python
import ctypes, ctypes.util
libc = ctypes.cdll.LoadLibrary(ctypes.util.find_library('c'))

class A:
    def __init__(self):
        print 'Creating an instance of A'

    def __del__(self):
        print 'Deleting an instance of A'
        #libc.memset(0, 0, 4)

for i in xrange(10):
    A()



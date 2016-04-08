#! /usr/bin/env python

v = {'LocalVariable': 'a', 'AlphaLocalVariables': 2}
for i in xrange(2):
    locals().update(v)
    print LocalVariable
    print AlphaLocalVariables
    for j in xrange(2):
        print LocalVariable
        print AlphaLocalVariables


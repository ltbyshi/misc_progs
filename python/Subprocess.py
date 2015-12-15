#! /usr/bin/env python

import subprocess, sys, os
import shlex

if len(sys.argv) < 2:
    print >>sys.stderr, 'Usage: %s command'%sys.argv[0]
    sys.exit(1)

command = sys.argv[1:]
print 'Command: ' + ' '.join(command)
returncode = 0
try:
    p = subprocess.Popen(command, shell=False)
    print 'Started: %d'%p.pid
    p.communicate()
    returncode = p.returncode
except OSError as e:
    print 'Failed to run command: %s'%e.strerror
    returncode = -e.errno

print 'Exited with %d'%returncode


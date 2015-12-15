#! /usr/bin/env python
from signal import *
import sys
import time

def handle_signal(signum, frame):
    print 'Received signal %d'%signum
    sys.exit(-signum)

for signum in (SIGINT, SIGPIPE, SIGTERM, SIGSEGV, SIGALRM):
    signal(signum, handle_signal)
while True:
    print 'yes'
time.sleep(100)


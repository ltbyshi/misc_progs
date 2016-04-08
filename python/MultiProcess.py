#! /usr/bin/env python

import multiprocessing
import os, sys
import time

def ExitHandler(signum, frame):
    import ctypes
    ctypes.memset(0, 1, 1)

def InitWorker():
    import signal
    signal.signal(signal.SIGINT, ExitHandler)

def MultiProcess():
    pool = multiprocessing.Pool(initializer=InitWorker)
    try:
        pool.map(time.sleep, [100]*100)
    except:
        print >>sys.stderr, 'Interrupted'
        pool.terminate()
    pool.close()
    pool.join()

if __name__ == '__main__':
    try:
        MultiProcess()
    except KeyboardInterrupt:
        sys.exit(1)
        print >>sys.stderr, "Interrupted"

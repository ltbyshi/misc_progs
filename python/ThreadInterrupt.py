#! /usr/bin/env python

import threading
import os, sys, time, signal

def HandleSignal(signum, frame):
    if signum == signal.SIGINT:
        print frame
        sys.exit(1)

class MyThread(threading.Thread):
    def run(self):
        time.sleep(100)

signal.signal(signal.SIGINT, signal.SIG_DFL)
signal.signal(signal.SIGINT, HandleSignal)
t = MyThread();
t.start()
time.sleep(100)
t.join()


from textcolor import tcolor as _tcolor
import sys as _sys
import os as _os

colored = True
verbose = True

if _os.isatty(1) and _os.isatty(2):
    _tcolor.Enable(colored)
else:
    _tcolor.Enable(False)

def log(message):
    if verbose:
        print message

def warn(message):
    '''Display warning message and proceed
    '''
    if _sys.stderr.closed:
        return
    _sys.stderr.write(_tcolor.Warn('Warning: '))
    _sys.stderr.write(_tcolor.WarnMessage(message) + '\n')
    
def error(message):
    '''Display error message and exit
    '''
    if _sys.stderr.closed:
        return
    _sys.stderr.write(_tcolor.Warn('Error: '))
    _sys.stderr.write(_tcolor.WarnMessage(message) + '\n')
    
def info(title, message, use_stderr=False):
    f = _sys.stdout
    if use_stderr:
        f = _sys.stderr
    if verbose and (not f.closed) or use_stderr:
        f.write(_tcolor.Info(title + ': '))
        f.write(_tcolor.Message(message) + '\n')
 

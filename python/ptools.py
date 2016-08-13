import os as _os
import subprocess as _subprocess
import array as _array
import shutil as _shutil
from textcolor import tcolor as _tcolor
import sys as _sys

verbose = True
dryRun = False
mute = True
colored = True

if _os.isatty(1) and _os.isatty(2):
    _tcolor.Enable(colored)
else:
    _tcolor.Enable(False)
    
class ExecuteError(Exception):
    pass

def Log(message):
    if verbose:
        print message

def ShowWarning(message):
    '''Display warning message and proceed
    '''
    if _sys.stderr.closed:
        return
    _sys.stderr.write(_tcolor.Warn('Warning: '))
    _sys.stderr.write(_tcolor.WarnMessage(message) + '\n')
    
def ShowError(message):
    '''Display error message and exit
    '''
    if _sys.stderr.closed:
        return
    _sys.stderr.write(_tcolor.Warn('Error: '))
    _sys.stderr.write(_tcolor.WarnMessage(message) + '\n')
    raise ExecuteError(message)
    
def ShowInfo(title, message, use_stderr=False):
    f = _sys.stdout
    if use_stderr:
        f = _sys.stderr
    if verbose and (not f.closed) or use_stderr:
        f.write(_tcolor.Info(title + ': '))
        f.write(_tcolor.Message(message) + '\n')
    
def GetNullDevice():
    return open('/dev/null', 'w')

#Execute external command with arguments
#Return the standard output as a string
def Execute(args, inData=None):
    args = [str(arg) for arg in args]
    cmdLine = ' '.join(args)
    if dryRun:
        Log(_tcolor.Info('Execute: ') +
        _tcolor.Message(cmdLine))
        return

    stdout = None
    if mute:
        stdout = GetNullDevice()
    if(inData):
        p = _subprocess.Popen(args, stdin=_subprocess.PIPE, stdout=stdout)
        
        ShowInfo('Execute[%s]'%p.pid, cmdLine)
        
        p.communicate(inData)
    else:
        p = _subprocess.Popen(args, stdout=stdout)
        
        ShowInfo('Execute[%s]'%p.pid, cmdLine)
        
        exitCode = p.wait()
        if exitCode != 0:
            raise ExecuteError('program exited with %d'%exitCode)

def ShellExecute(cmd):
    ShowInfo('ShellExec', cmd)
    status = True
    if not dryRun:
        exitCode = _os.system(cmd)
        if exitCode != 0:
            ShowWarning('program exited with %d'%exitCode)
            status = False
    return status

#Move files
def MoveFile(srcFile, destFile):
    ShowInfo('MoveFile', '%s => %s'%(srcFile, destFile))
    if not dryRun:
        _os.rename(srcFile, destFile)

#Copy files
def CopyFile(srcFile, destFile):
    ShowInfo('CopyFile', '%s => %s'%(srcFile, destFile))
    if not dryRun:
        _shutil.copyfile(srcFile, destFile)
        
def MakeDir(dirName):
    if not _os.path.exists(dirName):
        ShowInfo('MakeDir', dirName)
        if not dryRun:
            _os.makedirs(dirName)
            
def ChDir(dirName):
    ShowInfo('ChDir', dirName)
    if _os.path.exists(dirName):
        _os.chdir(dirName)
    else:
        ShowError('Failed to change directory')
        
def RmDir(dirName):
    ShowInfo('RmDir', dirName)
    if not dryRun:
        if _os.path.exists(dirName):
            try:
                _shutil.rmtree(dirName)
            except OSError:
                ShowError('Failed to remove directory')
        else:
            ShowError('Failed to remove directory')
        
def RemoveFile(fname):
    ShowInfo('RemoveFile', fname)
    if _os.path.isfile(fname):
        _os.remove(fname)
    
#Class for parsing .dot file
class Dot:
    def __init__(self, fname):
        lines = open(fname, 'rt').readlines()
        self.name = lines[0].strip()
        self.seq = lines[1].strip()
        self.struct = lines[2].strip()

class Stack:
    def __init__(self):
        self.data = []
    
    def pop(self):
        item = self.data[-1]
        self.data.pop()
        return item
    
    def push(self, item):
        if len(self.data) > 10000:
            raise ValueError('Stack overflow!')
        self.data.append(item)
        
    def top(self):
        return self.data[-1]
    
    def empty(self):
        return len(self.data) <= 0
        
def MakePairTable(structure):
    '''Zero elements are unpaired,
    non-negative elements are the partners
    '''
    S = Stack()
    N = len(structure)
    pairTable = [0 for i in xrange(N)]
    for i in xrange(N):
        if structure[i] == '(':
            S.push(i)
        elif structure[i] == ')':
            pairTable[S.top()] = i
            pairTable[i] = S.top()
            S.pop()
        else:
            pairTable[i] = -1
    return pairTable

def DotToBPSEQ(dotFile, bpseqFile):
    '''Convert .dot format to BPSEQ format (CONTRAFold)
    '''
    dot = Dot(dotFile)
    ptable = MakePairTable(dot.struct)
    N = len(dot.seq)
    with open(bpseqFile, 'wt') as f:
        for i in xrange(N):
            pairPos = (ptable[i] + 1) if ptable[i] > 0 else 0
            f.write('%s %s %s\n'%(i + 1, dot.seq[i], pairPos))

def IsBasePair(a, b):
    BASEPAIRS = [('A', 'U'), ('U', 'A'), ('G', 'U'), ('U', 'G'), ('C', 'G'), ('G', 'C')]
    return (a, b) in BASEPAIRS

def MakePairSet(structure):
    '''Return a set of base pairs (i, j) (0 based) of structure
    '''
    S = Stack()
    N = len(structure)
    pairSet = set()
    for i in xrange(N):
        if structure[i] == '(':
            S.push(i)
        elif structure[i] == ')':
            if not S.empty():
                pairSet.add((S.pop(), i))
            else:
                raise ValueError('Invalid RNA structure')
        else:
            pass
    return pairSet

def CalcScore(refDotFile, predDotFile):
    '''Return (Sensitivity, PPV) of the comparison
    '''
    refDot = Dot(refDotFile)
    predDot = Dot(predDotFile)
    refSet = MakePairSet(refDot.struct)
    predSet = MakePairSet(predDot.struct)
    comSet = refSet & predSet
    '''print 'Reference dot file:', refDotFile
    print 'Common pair set:', len(comSet)
    print 'Predicted pair set:', len(predSet)
    print 'Reference pair set:', len(refSet)'''
    if len(refSet) > 0:
        Sensitivity = float(len(comSet)) / float(len(refSet)) * 100.0
    else:
        Sensitivity = 0.0
    if len(predSet) > 0:
        PPV = float(len(comSet)) / float(len(predSet)) * 100.0
    else:
        PPV = 0.0
        
    return (Sensitivity, PPV)

def ArgMin(x):
    '''return the index of the minumum
    x: list
    '''
    minInd = 0
    minVal = x[0]
    for i in xrange(len(x)):
        if x[i] < minVal:
            minInd = i
            minVal = x[i]
    return minInd

def ArgMax(x):
    '''return the index of the maximum
    '''
    maxInd = 0
    maxVal = x[0]
    for i in xrange(len(x)):
        if x[i] > maxVal:
            maxInd = i
            maxVal = x[i]
    return maxInd

def ReadParams(fname):
    '''return a dict of param values
    '''
    params = {}
    with open(fname, 'rt') as f:
        for line in f:
            fields = line.strip().split()
            name = None
            value = None
            if len(fields) == 3:
                name, paramType, value = fields
                if(paramType == 'int'):
                    value = int(value)
                elif (paramType == 'float'):
                    value = float(value)
                else:
                    ShowError('Invalid param type "%s" in file "%s"'%(paramType, fname))
                params[name] = value
            else:
                ShowError('Param file "%s" does not have 3 fields in each line'%fname)
    return params

def Dequote(s):
    if (len(s) > 1) and s.startswith('"') and s.endswith('"'):
        return s[1:-1]
    else:
        return s
    
def RemoveExt(fname, ext=None, delim='.'):
    '''return file name with extension removed
    '''
    pos = fname.rfind(delim)
    if pos >= 0:
        return fname[0:pos]
    else:
        return fname
    
import os
import sys
import re
from ptools import MakeDir, ShowInfo, ShowWarning
import subprocess

class ArgumentError(Exception):
    def __init__(self, name, reason, other=''):
        self.name = name
        self.reason = reason
        self.other = other
        
    def __str__(self):
        msg = 'program argument %s is %s'%(repr(self.name), self.reason)
        if self.other:
            msg += ': ' + self.other
        return msg
    
class AppRunner(object):
    executable = ''
    # if executable is a script, the interpreter is used
    interpreter = None
    # argname: (option, nargs, required)
    option_dict = {}
    verbose = True
    positional_first = False
    
    def __init__(self, *args, **kwargs):
        self.kwargs = kwargs
        self.args = args
        
        self.stdin = None
        self.stdout = None
        self.stderr = None
        # handle to child process
        self._child = None
        
    def __str__(self):
        return ' '.join(self.getcmd())
    
    def getcmd(self):
        cmd = []
        if self.interpreter:
            cmd.append(self.interpreter)
        cmd.append(self.executable)

        options = []
        for key, value in self.kwargs.iteritems():
            if isinstance(value, bool):
                if value:
                    options.append(self.option_dict[key][0])
                continue
            options.append(self.option_dict[key][0])
            if hasattr(value, '__iter__'):
                for v in value:
                    options.append(str(v))
            else:
                options.append(str(value))
        if self.positional_first:
            cmd += self.args
            cmd += options
        else:
            cmd += options
            cmd += self.args
        return cmd
    
    def run(self, stdin=None, stdout=None, stderr=None, cwd=None, parse=False):
        '''stdin: a string or file object.
        stdout: a string or file object. 
        If stdout is a string, it is assumed to be an output file name.
        '''
        self.cmd = self.getcmd()
        if self.verbose:
            ShowInfo('ExecuteApp', str(self))

        if isinstance(stdout, str) | isinstance(stdout, unicode):
            stdout = open(stdout, 'w')
        if isinstance(stdout, str) | isinstance(stderr, unicode):
            stdout = open(stderr, 'w')
        if stdin:
            # using pipes
            if isinstance(stdin, file):
                self._child = subprocess.Popen(self.cmd, stdin=stdin,
                                   stdout=stdout, stderr=stderr, cwd=cwd)
                self.stdout, self.stderr = self._child.communicate()
            else:
                self._child = subprocess.Popen(self.cmd, stdin=subprocess.PIPE,
                                   stdout=stdout, stderr=stderr, cwd=cwd)
                self.stdout, self.stderr = self._child.communicate(stdin)
        else:
            self._child = subprocess.Popen(self.cmd, stdout=stdout, stderr=stderr, cwd=cwd)
            self.stdout, self.stderr = self._child.communicate()
        
        self.returncode = self._child.returncode
        # non-zero exit code
        if self.returncode != 0:
            ShowWarning('%s exits with code %d'%(self.executable, self.returncode))
        
        if parse:
            return self.parse()
        return self
    
    def parse(self):
        '''Parse stdout or output files of the program and returns a parsed object.
        Default is to return the stdout of the program.
        '''
        return self.stdout

def create_app(executable, option_dict=None, *args, **kwargs):
    '''create a simple app from scratch
    '''
    app = AppRunner(*args, **kwargs)
    app.executable = executable
    if option_dict:
        app.option_dict = option_dict
    return app

class Pipeline:
    verbose = True
    
    def __init__(self, *apps):
        if len(apps) < 2:
            raise ValueError('A pipeline should contain at least 2 applications')
        for app in apps:
            if not isinstance(app, AppRunner):
                raise ValueError('Arguments passed to Pipeline should be instances of AppRunner')
        self.apps = apps
        
    def __str__(self):
        return ' | '.join([' '.join(app.getcmd()) for app in self.apps])
    
    def run(self, stdin=None):
        if self.verbose:
            ShowInfo('ExecutePipeline', str(self))
        children = []
        # add first app
        if stdin:
            if isinstance(stdin, file):
                child = subprocess.Popen(self.apps[0].getcmd(), stdin=stdin,
                            stdout=subprocess.PIPE, stderr=subprocess.PIPE)
            else:
                child = subprocess.Popen(self.apps[0].getcmd(), stdin=subprocess.PIPE,
                            stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        else:
            child = subprocess.Popen(self.apps[0].getcmd(),
                            stdout=subprocess.PIPE, stderr=subprocess.PIPE)
        children.append(child)
        # add remaining apps
        for i in xrange(1, len(self.apps)-1):
            child = subprocess.Popen(self.apps[i].getcmd(), stdin=children[i-1].stdout,
                            stdout=subprocess.PIPE, stderr=subprocess.PIPE)
            children[-1].stdout = child.stdin
            children.append(child)
        # add last app
        children.append(subprocess.Popen(self.apps[-1].getcmd(), stdin=children[-1].stdout,
                            stderr=subprocess.PIPE))
        children[-2].stdout = children[-1].stdin
        # executing the pipeline
        # tuples of (stdout, stderr, returncode)
        rundata = []
        # execute first app
        if stdin and (not isinstance(stdin, file)):
            stdout, stderr = children[0].communicate(stdin)
        else:
            stdout, stderr = children[0].communicate(stdin)
        rundata.append((stdout, stderr, children[0].returncode))
        # execute remaining apps
        for i in xrange(1, len(self.apps)):
            stdout, stderr = children[i].communicate()
            rundata.append((stdout, stderr, children[i].returncode))
        
        if rundata[-1][2] != 0:
            ShowWarning('%s exits with code %d'%(self.apps[-1].executable, rundata[-1][2]))
        # error message
        for i in xrange(len(self.apps)):
            stderr = rundata[i][1]
            if stderr:
                sys.stderr.write('\x1B[31;1m' + '[%s] \x1B[0m \x1B[31mError Message:\n'%self.apps[i].executable)
                sys.stderr.write(stderr + '\x1B[0m\n')
        
class wc(AppRunner):
    executable = 'wc'
    option_dict = {'l': ('-l', 0)}
    
    def run(self, stdin=None, stdout=None, stderr=None, parse=False):
        if not stdout:
            stdout = subprocess.PIPE
        return AppRunner.run(self, stdin, stdout, stderr, parse=True)
        
    def parse(self):
        fields = self.stdout.strip().split()
        if self.kwargs.has_key('l'):
            return int(fields[0])
        else:
            return (int(fields[0]), int(fields[1]), int(fields[2]))
        
class cat(AppRunner):
    executable = 'cat'

class java(AppRunner):
    executable = 'java'
    
class Sfold(AppRunner):
    executable = 'sfold'
    option_dict = {'a': ('-a', 1),
                'f': ('-f', 1),
                'l': ('-l', 1),
                'm': ('-m', 1),
                'o': ('-o', 1, True),
                'w': ('-w', 1),
                'e': ('-e', 0),
                'i': ('-i', 1)}
        
    def __init__(self, *args, **kwargs):
        AppRunner.__init__(self, args, kwargs)
        self.outdir = self.kwargs['o']
    
    def parse_sstrand(self):
        import pandas as pd
        return pd.read_table(self.outdir + '/sstrand.out',
                       header=None, sep=r'\s+', engine='python',
                       names=['pos', 'nuc', 'cnuc', 'prob1', 'prob2', 'prob3'])
    
    def parse_loopr(self):
        import pandas as pd
        return pd.read_table(self.outdir + '/loopr.out',
                       header=None, sep=r'\s+', engine='python',
                       names=['pos', 'nuc', 'probH', 'probB', 'probI', 'probM', 'probE', 'total'])

class MEME(AppRunner):
    executable = 'meme'
    positional_first = True
    option_dict = {'o': ('-o', 1),
                'oc': ('-oc', 1),
                'text': ('-text', 0),
                'dna': ('-dna', 0),
                'mod': ('-mod', 1),
                'w': ('-w', 1),
                'minw': ('-minw', 1),
                'maxw': ('-maxw', 1),
                'p': ('-p', 1),
                'maxsize': ('-maxsize', 1),
                'nmotifs': ('-nmotifs', 1)}
    pssm_pat = re.compile(r'^\s+Motif [0-9]+ position-specific probability matrix\s*\n-+\n(?P<motif>(.+\n)+)-+$', re.M)
    sites_pat = re.compile(r'^\s+Motif [0-9]+ sites sorted.+\n^-.+\nSequence name.+\n-.+\n(?P<sites>(.+\n)+)-+$', re.M)
    alphabet_pat = re.compile(r'^(ALPHABET= .+\n)', re.M)
    strands_pat = re.compile(r'^(strands: .+\n)', re.M)
    bg_pat = re.compile(r'^(Background letter frequencies).+(\n.+\n)', re.M)
    
    class PSPM:
        def __init__(self, text):
            lines = text.split('\n')
    
    def __init__(self, *args, **kwargs):
        AppRunner.__init__(self, *args, **kwargs)
        if self.kwargs.has_key('o'):
            self.outdir = self.kwargs['o']
        if self.kwargs.has_key('oc'):
            self.outdir = self.kwargs['oc']
            
    def to_minimotif(self, fname=None):
        fout = sys.stdout
        if fname:
            fout = open(fname, 'w')
        text = open(os.path.join(self.outdir, 'meme.txt'), 'r').read()
        fout.write('MEME version 4\n\n')
        fout.write(self.alphabet_pat.search(text).group(1) + '\n')
        fout.write('strands: +\n\n')
        #fout.write(self.strands_pat.search(text).group(1) + '\n')
        fout.write(''.join(self.bg_pat.search(text).group(1, 2)) + '\n')
        
        nmotifs = 1
        for m in self.pssm_pat.finditer(text):
            fout.write('MOTIF %d\n'%nmotifs)
            fout.write(m.group('motif') + '\n')
            nmotifs += 1
        
        if fname:
            fout.close()
    def parse(self):
        text = open(os.path.join(self.outdir, 'meme.txt'), 'r').read()
        motifs = []
        for m in self.pssm_pat.finditer(text):
            motifs.append(m.group('motif'))
        return motifs
    
    def parse_sites(self):
        '''return a list of sites for each motif
        For example, sites[i][j] is the jth site for the ith motif.
        Each site record is a tuple of (name, 1-based start position, p-value, 
            upstream sequence, motif site sequence, downstream sequence).
        '''
        text = open(os.path.join(self.outdir, 'meme.txt'), 'r').read()
        sites = []
        for m in self.sites_pat.finditer(text):
            sites_text = m.group('sites')
            sites_motif = []
            for line in sites_text.strip().split('\n'):
                fields = line.split()
                sites_motif.append((fields[0], int(fields[1]), float(fields[2]),
                                    fields[3], fields[4], fields[5]))
            sites.append(sites_motif)
        return sites
    
class FIMO(AppRunner):
    executable = 'fimo'
    option_dict = {'o': ('--o', 1), 'oc': ('--oc', 1), 'text': ('--text', 0),
                'motif': ('--motif', 1), 'thresh': ('--thresh', 1),
                'no_qvalue': ('--no-qvalue', 0), 'qv_thresh': ('--qv-thresh', 1)}
    
    def parse(self):
        import pandas as pd
        if self.kwargs.has_key('o'):
            self.outdir = self.kwargs['o']
        if self.kwargs.has_key('oc'):
            self.outdir = self.kwargs['oc']
        results = pd.read_table(os.path.join(self.outdir, 'fimo.txt'), skiprows=1, 
                names=['motif_id', 'name', 'start', 'stop', 'strand', 'score', 'pvalue', 'qvalue', 'sequence'])
        return results
    
class MAST(AppRunner):
    executable = 'mast'
    option_dict = {'o': ('-o', 1), 'oc': ('-oc', 1), 'm': ('-m', 1),
                'hit_list': ('-hit_list', 0)}
    
    
class RNAcontext(AppRunner):
    executable = 'rnacontext'
    option_dict = {'a': ('-a', 1), 'e': ('-e', 1), 'c': ('-c', 1),
                'd': ('-d', 1), 'h': ('-h', 1), 'n': ('-n', 1),
                'o': ('-o', 1), 's': ('-s', 1)}
    
    def __init__(self, *args, **kwargs):
        if not kwargs.has_key('outdir'):
            raise ArgumentError('outdir', 'missing')
        self.outdir = kwargs['outdir']
        del kwargs['outdir']
        AppRunner(self, *args, **kwargs)
    
    def run(self, stdin=None, stdout=None, stderr=None, cwd=None, parse=False):
        MakeDir(self.outdir)
        AppRunner.run(self, stdin, stdout, stderr, self.outdir, parse)
    
    
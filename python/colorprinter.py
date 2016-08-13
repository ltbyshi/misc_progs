import sys
import itertools

RESET = 0
BOLD = 1
DIM = 2
UNDERLINE = 3
BLINK = 4
REVERSE = 7
HIDDEN = 8
# color numbers
BLACK = 30
RED = 31
GREEN = 32
YELLOW = 33
BLUE = 34
MAGENTA = 35
CYAN = 36
WHITE = 37

class ColorPrinter:
    def __init__(self, fout):
        self.__dict__['fout'] = fout
        self.__dict__['bg'] = None
        self.__dict__['attr'] = None
        self.__dict__['fg'] = None
        
    def reset(self):
        self.__dict__['bg'] = None
        self.__dict__['attr'] = None
        self.__dict__['fg'] = None
        
    def setcolor(self, fg=None, bg=None, attr=None):
        if fg:
            self.__dict__['fg'] = bg
        if bg:
            self.__dict__['bg'] = bg
        if attr:
            self.__dict__['attr'] = attr
            
    def colorcode(self, reset=False):
        if reset:
            return '\x1B[0m'
        else:
            return '\x1B[' + ';'.join([str(a) for a in (self.bg, self.attr, self.fg) if a]) + 'm'
        
    def write(self, s):
        if s.endswith('\n'):
            self.fout.write(self.colorcode() + s[:-1] + self.colorcode(True) + '\n')
        else:
            self.fout.write(self.colorcode() + s + self.colorcode(True))
        
    def writelines(self, sequence):
        self.fout.write(self.colorcode())
        self.fout.writelines(sequence[:-1])
        self.fout.write(sequence[-1] + self.colorcode(True) + '\n')
        
    def __getattr__(self, attr):
        return getattr(self.fout, attr)
    
    def __setattr__(self, attr, value):
        return setattr(self.fout, attr, value)
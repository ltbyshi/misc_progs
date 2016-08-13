# text color utility

class TextColor:
    # color attributes
    RESET = 0
    BOLD = 1
    DIM = 2
    UNDERLINE = 3
    BLINK = 4
    REVERSE = 7
    HIDDEN = 8
    # color numbers
    BLACK = 0
    RED = 1
    GREEN = 2
    YELLOW = 3
    BLUE = 4
    MAGENTA = 5
    CYAN = 6
    WHITE = 7
    
    def __init__(self):
        self._enabled = True
    def Enable(self, enabled):
        self._enabled = enabled
    # end codes
    def End(self):
        if self._enabled:
            return '\x1B[0m'
        else:
            return ''
    def GetCode(self, fg, attr, bg):
        if self._enabled:
            return '\x1B[%d;%d;%dm'%(fg + 30, attr, bg + 40)
        else:
            return ''
    def Warn(self, text):
        if self._enabled:
            return '\x1B[%d;%dm%s\x1B[0m'%(TextColor.RED + 30, TextColor.BOLD, text)
        else:
            return str(text)
    def WarnMessage(self, text):
        if self._enabled:
            return '\x1B[%dm%s\x1B[0m'%(TextColor.RED + 30, text)
        else:
            return str(text)
    def Info(self, text):
        if self._enabled:
            return '\x1B[%d;%dm%s\x1B[0m'%(TextColor.GREEN + 30, TextColor.BOLD, text)
        else:
            return str(text)
    def Message(self, text):
        if self._enabled:
            return '\x1B[%dm%s\x1B[0m'%(TextColor.YELLOW + 30, text)
        else:
            return str(text)

tcolor = TextColor()

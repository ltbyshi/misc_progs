#! /usr/bin/env python

import string
import re
from collections import defaultdict

class VarTemplate(string.Template):
    pattern = r'''
    \$(?:
      (?P<escaped>\$) |   # Escape sequence of two delimiters
      (?P<named>[_a-z][_a-z0-9]*)      |   # delimiter and a Python identifier
      {(?P<braced>[_a-z][._a-z0-9]*)}   |   # delimiter and a braced identifier
      (?P<invalid>)              # Other ill-formed delimiter exprs
    )'''
 
class Namespace(object):
    key_pat = re.compile(r'^([_a-zA-Z][_a-z0-9]*)(\.[_a-z][_a-z0-9]*)*$', re.I)
    
    def __init__(self, name='', parent=None):
        self.name = name
        self.parent = parent
        self.children = {}
        
    def _getitem(self, tokens):
        name = tokens[0]
        if not self.children.has_key(name):
            raise KeyError('Variable or namespace \'%s\' does not exist'%name)
        
        if len(tokens) == 1:
            return self.children[name]
        else:
            return self.children[name]._getitem(tokens[1:])
        
    def __getitem__(self, key):
        if not Namespace.key_pat.match(key):
            raise KeyError('\'%s\' is not a valid key'%key)
        # get namespaces
        tokens = key.split('.')
        return self._getitem(tokens)
    
    def _setitem(self, tokens, value):
        name = tokens[0]
        
        if len(tokens) == 1:
            # prevent from overwriting a namespace by a variable
            if self.children.has_key(name):
                child = self.children[name]
                if isinstance(child, Namespace):
                    raise KeyError('Namespace \'%s\' already exists and cannot be overwritten'%tokens[0])
                else:
                    self.children[name] = value
            else:
                # set value
                self.children[name] = value
        else:
            child = None
            if self.children.has_key(tokens[0]):
                child = self.children[tokens[0]]
            else:
                child = Namespace(tokens[0], self)
                self.children[tokens[0]] = child
            child._setitem(tokens[1:], value)
        
    def __setitem__(self, key, value):
        if not Namespace.key_pat.match(key):
            raise KeyError('\'%s\' is not a valid key'%key)
        if not (isinstance(value, str) or isinstance(value, unicode)):
            raise KeyError('The key is neither a str nor unicode instance')
        # get namespaces
        tokens = key.split('.')
        self._setitem(tokens, value)
        
    def fullname(self):
        if not self.parent:
            return self.name
        else:
            names = [self.name]
            parent = self.parent
            while parent:
                names.append(parent.name)
                parent = parent.parent
            return '.'.join(reversed(names[:-1]))
        
    def _str(self):
        lines = []
        # print namespace
        fullname = self.fullname()
        if len(self.children) or len(self.data):
            lines.append(fullname + ':\tNamespace')
        else:
            lines.append(fullname + ':\tEmpty namespace')
        
        # print children
        for key in self.children.iterkeys():
            child = self.children[key]
            if isinstance(child, Namespace):
                lines += self.children[key]._str()
            else:
                if fullname:
                    lines.append('%s.%s:\t\'%s\''%(fullname, key, child))
                else:
                    lines.append('%s:\t\'%s\''%(key, child))    
        return lines
        
    def __str__(self):
        return '\n'.join(self._str())
    
class ConfigManager(dict):
    max_sub_levels = 30
    # valid key names
    key_pat = re.compile(r'^([_a-z][_a-z0-9]*)(\.[_a-z][_a-z0-9]*)*$', re.I)
    
    def __init__(self):
        dict.__init__(self)
        # stores reference count to each namespace
        self.namespaces = defaultdict(int)
        
    def __getitem__(self, key):
        #resolve variables of type str or unicode
        val = dict.__getitem__(self, key)
        t = VarTemplate(val)
        levels = 0
        newval = None
        status = False
        oldval = str(val)
        while levels < ConfigManager.max_sub_levels:
            t = VarTemplate(oldval)
            newval = t.substitute(self)
            if newval == oldval:
                status = True
                break
            oldval = newval
            levels += 1
        if not status:
            raise KeyError('\'%s\' cannot be resolved: maximum substitution levels reached'%key)
        return newval
    
    def __setitem__(self, key, value):
        if not ConfigManager.key_pat.match(key):
            raise KeyError('\'%s\' is not a valid variable name'%key)
        if not (isinstance(value, str) or isinstance(value, unicode)):
            raise KeyError('The key is neither a str nor unicode instance')
        # get namespace
        fields = key.split('.')
        if len(fields) > 1:
            ns = '.'.join(fields[:-1])
            self.namespaces[ns] += 1
        dict.__setitem__(self, key, value)
        
    def __delitem__(self, key):
        # find the namespace
        the_ns = None
        for ns in self.namespaces.iterkeys():
            if len(key) > len(ns):
                if key.startswith(ns) and key[len(ns)] == '.':
                    self.namespaces[ns] -= 1
                    the_ns = ns
                    break
        
        dict.__delitem__(self, key)
        
    def remove_unused_ns(self):
        for ns in self.namespaces.keys():
            if self.namespaces[ns] < 1:
                del self.namespaces[ns]
            
    def use(self, *args):
        '''Import all variables from namespace ns
        Namespaces are seperated by '.'.
        '''
        for ns in args:
            if ns not in self.namespaces.iterkeys():
                raise KeyError('Namespace \'%s\' does not exist'%ns)
            newkey = None
            for key in self.iterkeys():
                if (len(key) > len(ns)) and key.startswith(ns):
                    newkey = key[len(ns)+1:]
                    break
            if newkey:
                dict.__setitem__(self, newkey, dict.__getitem__(self, key))
    
    def add_ns(self, ns):
        '''Add a new namespace if ns does not exist
        '''
        if not self.namespaces.has_key(ns):
            self.namespaces[ns] = 0
        
    def __str__(self):
        lines = []
        for key in self.keys():
            lines.append(key + ':\t' + dict.__getitem__(self, key))
        return '\n'.join(lines)
    
def TestNamespace():
    ns = Namespace()
    ns['A'] = 'hello'
    ns['B.A.C'] = 'world'
    ns['B.B'] = 'foo'
    ns['C.D'] = 'bar'
    ns['B.A.C'] = 'sick'
    ns['C.E.B'] = 'stack'
    
    print ns
    
def TestConfig():
    config = ConfigManager()
    config['A'] = 'hello'
    config['B'] = 'world'
    config['C'] = 'foo'
    config['A.aa'] = '${A} ${B}'
    config['B.bb'] = '${A} ${C}'
    config['C.cc'] = '${A} ${B} ${C}'
    config['A.B.aa'] = 'bar'
    
    del config['C.cc']
    print 'Namespaces:', config.namespaces
    config.remove_unused_ns()
    print 'Namespaces:', config.namespaces
    config.add_ns('D')
    print 'Namespaces:', config.namespaces
    
    config.use('A', 'B')
    print config
    
if __name__ == '__main__':
    TestNamespace()
    
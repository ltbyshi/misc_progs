#! /usr/bin/env python

types = ('char', 'unsigned char', 'short','unsigned short',
    'int', 'unsigned int', 'long', 'unsigned long',
    'long long', 'unsigned long long',
    'float', 'double', 'long double',
    'std::string')

def GenMacro(proto, action):
    print proto + ' \\'
    for i in xrange(len(types)):
        code = ['    ']
        if i > 0:
            code.append('else ')
        code.append('if(ti == typeid(%s)){'%types[i])
        code.append(action.format(t=types[i]))
        code.append('} \\')
        print ''.join(code)
    print '    else{defstmt;}'

print '#ifndef __VOIDTYPE_H__'
print '#define __VOIDTYPE_H__'

GenMacro('#define CAST_VOID(ti, psrc, pdest, stmt, defstmt)',
        '{t}* pdest = ({t}*)psrc; stmt;')
GenMacro('#define NEW_VOID(ti, pdest, defstmt)', 'pdest = new {t};')
GenMacro('#define NEW_VOID_ARRAY(ti, ptr, n, defstmt)', 'ptr = new {t}[n];')
GenMacro('#define DELETE_VOID(ti, ptr, defstmt)', 'delete ({t}*)ptr;')
GenMacro('#define DELETE_VOID_ARRAY(ti, ptr, defstmt)', 'delete[] ({t}*)ptr;')

print '#endif'

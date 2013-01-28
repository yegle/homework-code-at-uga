#!/usr/bin/env python
# vim: set fileencoding=utf-8 :

from __future__ import (unicode_literals, absolute_import,
                        division, print_function)

import re
import sys
from ctypes import *

lexer = cdll.LoadLibrary("./libtjlex.so")

# Python2/3 compatible functions
def _b(text):
    if sys.version_info[0] == 2:
        return bytes(text)
    else:
        return bytes(text, 'utf-8')

# a simple wrapper function around the original yylex C function
def yylex(filename):
    filename = _b(filename)
    print(filename)
    ret = lexer.set_file(filename)
    if ret == -1:
        return
    while True:
        tok = lexer.yylex()
        if tok:
            yield (tok, c_char_p.in_dll(lexer, 'yytext').value)
        else:
            break

# parsing the header file to get the symbol table
def get_symbols(header_file):
    d = dict()
    pattern = r'[ \t]+'
    for line in open(header_file):
        line = re.sub(pattern, ' ', line)
        _, symbol, value = line.strip().split(' ')
        d[int(value)] = symbol
    return d

if __name__ == '__main__':
    symbols = get_symbols('y.tab.h')
    for token in yylex('sample.java'):
        s = symbols[token[0]]
        if s == 'ERROR':
            reason = c_char_p.in_dll(lexer, 'reason').value
            lineno = c_int.in_dll(lexer, 'yylineno').value
            print("Error at line %s: %s" % (lineno, token[1]))
            print("REASON: %s" % (reason,))
            sys.exit(255)
        text = token[1].decode('utf-8')
        print('%s "%s"' % (s, text))

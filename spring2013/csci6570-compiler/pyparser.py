#!/usr/bin/env python
# vim: set fileencoding=utf-8 :

from __future__ import (unicode_literals, absolute_import,
                        division, print_function)

import re
import sys
from ctypes import *

parser = cdll.LoadLibrary("./libtjparse.so")

# Python2/3 compatible functions
def _b(text):
    if sys.version_info[0] == 2:
        return bytes(text)
    else:
        return bytes(text, 'utf-8')

# parsing the header file to get the symbol table
def get_symbols(header_file):
    d = dict()
    pattern = r'[ \t]+'
    def_regex = re.compile(r'^#define')
    for line in open(header_file):
        if def_regex.match(line):
            line = re.sub(pattern, ' ', line)
            _, symbol, value = line.strip().split(' ')
            d[int(value)] = symbol
    return d

def parse(filename):
    filename = _b(filename)
    print(filename)
    ret = parser.set_file(filename)
    if ret == -1:
        return

    return parser.yyparse()


if __name__ == '__main__':
    print(parse('sample.java'))

#!/usr/bin/env python
# vim: set fileencoding=utf-8 :

from __future__ import (unicode_literals, absolute_import,
                        division, print_function)

import argparse

class MyParser(object):

    def __init__(self, args):
        self.server = '8.8.8.8'
        for arg in args:
            if arg.startswith('@'):
                self.server = arg.strip('@')
            else:
                self.domain = arg.strip('.')

    def __call__(self):
        return self.domain, self.server

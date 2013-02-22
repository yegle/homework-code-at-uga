#!/usr/bin/env python
# vim: set fileencoding=utf-8 :

from __future__ import (unicode_literals, absolute_import,
                        division, print_function)

import argparse

class MyParser(object):

    def __init__(self, args):
        p = argparse.ArgumentParser(description='My DNS query client')
        p.add_argument('domain_name', help='Domain name to be queried')
        p.add_argument(
            'dns_server',
            help='DNS server to be used. Default: @8.8.8.8',
            default='@8.8.8.8',
            nargs='?',
        )
        self.p = p

    def __call__(self):
        return self.p.parse_args()

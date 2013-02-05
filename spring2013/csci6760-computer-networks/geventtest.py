#!/usr/bin/env python
# vim: set fileencoding=utf-8 :

from __future__ import (unicode_literals, absolute_import,
                        division, print_function)

from gevent import socket, pool

def make_connection(address):
    s = socket.socket()

    s.connect(address)
    return s

if __name__ == '__main__':
    c = 0
    p = pool.Pool()
    while True:
        p.spawn(make_connection, ('localhost',9000))
        try:
            p.join(raise_error=True)
            c += 1
        except Exception:
            print(c)
            break

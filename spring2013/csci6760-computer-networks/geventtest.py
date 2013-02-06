#!/usr/bin/env python
# vim: set fileencoding=utf-8 :

from __future__ import (unicode_literals, absolute_import,
                        division, print_function)

from gevent import socket, pool
from collections import Counter



def make_connection(local_addr, remote_addr):
    s = socket.socket()
    s.bind(local_addr)
    s.connect(remote_addr)
    global l
    l.append(s)
    global c
    c['count'] +=1

if __name__ == '__main__':
    c = Counter()
    p = pool.Pool()
    l = list()
    localaddr = [
        '127.0.0.1',
        '127.0.0.2',
        '127.0.0.3',
        '127.0.0.4',
        '127.0.0.5',
    ]
    while True:
        p.spawn(make_connection, (localaddr[c['count']%5], 0), ('127.0.0.1',9000))
        try:
            p.join(raise_error=True)
        except Exception:
            print(c['count'])
            break

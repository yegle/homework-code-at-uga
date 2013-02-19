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
    print(c['count'])

if __name__ == '__main__':
    c = Counter()
    p = pool.Pool()
    l = list()
    localaddr = [
        '10.0.0.6',
        '10.0.0.7',
        '10.0.0.8',
        '10.0.0.9',
        '10.0.0.10',
    ]
    while True:
        p.spawn(make_connection, (localaddr[c['count']%5], 0),
                ('10.0.0.2',9000))
        try:
            p.join(raise_error=True)
        except Exception:
            print(c['count'])
            break

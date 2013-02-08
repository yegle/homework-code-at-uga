#!/usr/bin/env python
# vim: set fileencoding=utf-8 :

from __future__ import (unicode_literals, absolute_import,
                        division, print_function)

from gevent import socket, pool
from collections import Counter
import time
import sys



def make_connection(local_addr, remote_addr, data):
    s = socket.socket()
    s.bind(local_addr)
    s.connect(remote_addr)
    s.send(data)
    s.recv(len(data)+2)
    s.close()
    global c
    c['count'] += len(data)

if __name__ == '__main__':
    try:
        server_addr = sys.argv[1]
        server_port = sys.argv[2]
        packet_size = sys.argv[3]
    except IndexError:
        print("Usage: %s <server_addr> <server_port> <packet_size>" %
              sys.argv[0])
        sys.exit(1)
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
    counter = 0
    start = time.time()
    c['count'] = 0
    while True:
        counter += 1
        if counter % 1000 == 0:
            end = time.time()
            print("%s, %s MB/s" % (c['count'], c['count']/(1024^2)))
            start = end
            c['count'] = 0
        p.spawn(make_connection, (localaddr[c['count']%5], 0),
                ('10.0.0.2',9000), b'A'*10240)
        try:
            p.join(raise_error=True)
        except Exception:
            break

#!/usr/bin/env python
# vim: set fileencoding=utf-8 :

from __future__ import (unicode_literals, absolute_import,
                        division, print_function)

from gevent import socket
from gevent.server import StreamServer

def str_echo(s, address):
    while True:
        line = s.recv(1024)
        if line.strip():
            s.send(line)
        else:
            break
    s.shutdown(socket.SHUT_WR)
    s.close()


if __name__ == '__main__':
    address = ('', 9000)
    server = StreamServer(address, str_echo)
    server.serve_forever()

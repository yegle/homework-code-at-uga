#!/usr/bin/env python
# vim: set fileencoding=utf-8 :

from __future__ import (unicode_literals, absolute_import,
                        division, print_function)

import os
import sys
import socket
import signal
import errno
import logging

def str_echo(fd):
    try:
        while True:
            b = fd.recv(1024)
            if b.strip():
                fd.send(b)
            else:
                break
    except socket.error as e:
        logging.error('str_echo read error')

def sig_chld(signo, ignored):
    try:
        pid, exit_status = os.waitpid(-1, 0)
    except Exception:
        pass

if __name__ == '__main__':
    listenfd = socket.socket(socket.AF_INET, socket.SOCK_STREAM, 0)
    servaddr = ('', 9000)
    listenfd.bind(servaddr)
    listenfd.listen(1)
    listenfd.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

    signal.signal(signal.SIGCHLD, sig_chld)

    while True:
        try:
            connfd, remote_addr = listenfd.accept()
        except socket.error as e:
            if e.errno == errno.EINTR:
                continue
            else:
                tools.err_sys(e, msg="accept error")

        try:
            if not os.fork():
                listenfd.close()
                str_echo(connfd)
                connfd.close()
                sys.exit(0)
        except OSError:
            pass

        connfd.close()

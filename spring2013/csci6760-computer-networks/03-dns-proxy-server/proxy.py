#!/usr/bin/env python
# vim: set fileencoding=utf-8 :

from __future__ import (unicode_literals, absolute_import,
                        division, print_function)

import sys
import socket
import os
import logging
import struct

from server import DNSProxy
from server.timeout import TimeoutError

if __name__ == '__main__':
    logging.basicConfig(
        level=logging.INFO,
        format='%(asctime)s %(message)s')
    try:
        upstream = sys.argv[1]
        port = int(sys.argv[2])
    except IndexError:
        logging.warning(
            'Usage: %s UPSTREAM_IP_ADDR LOCAL_PORT' % (sys.argv[0],)
        )
        sys.exit(1)

    local_addr = ('', port)

    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    s.bind(local_addr)

    proxy = DNSProxy(upstream)

    logging.info('Listening on port %s, with upstream server %s' %
                 (port, upstream))

    while True:
        try:
            try:
                data, remote_addr = s.recvfrom(1024)
            except socket.error as e:
                if e.errno == errno.EINTR:
                    continue
                else:
                    logging.critical('Accept error, exiting')
                    sys.exit(1)
        except KeyboardInterrupt:
            logging.warning('Exiting proxy server')
            sys.exit(0)


        try:
            if not os.fork():
                try:
                    ret = proxy.proxy(data, remote_addr)
                    s.sendto(ret, remote_addr)
                    logging.info('Valid DNS query from %s' % remote_addr[0])
                except struct.error as e:
                    logging.warning('Invalid DNS query from %s' %
                                    (remote_addr[0]))
                except TimeoutError:
                    logging.warning(
                        'Timeout connecting to upstream server')
                except Exception as e:
                    logging.warning('Unknown error: %s' % str(e))
                sys.exit(0)
        except OSError:
            pass

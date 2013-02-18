#!/usr/bin/env python
# vim: set fileencoding=utf-8 :

from __future__ import (unicode_literals, absolute_import,
                        division, print_function)

import sys
import socket
from client.timeout import timeout
from client.utils import MyParser
from client.packet import ARecordQuery, DNSAnswer


if __name__ == '__main__':
    args = MyParser(sys.argv[1:])()
    domain, server = args.domain_name, args.dns_server

    q = ARecordQuery(domain)
    message = q.pack()

    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    s.bind(('', 10240))
    s.sendto(message, (server, 53))

    while True:
        result, data = s.recvfrom(1024)
        a = DNSAnswer(result).parse()
        print(a)

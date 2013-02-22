#!/usr/bin/env python
# vim: set fileencoding=utf-8 :

from __future__ import (unicode_literals, absolute_import,
                        division, print_function)

import sys
import socket
from client.timeout import timeout, TimeoutError
from client.utils import MyParser
from client.packet import ARecordQuery, DNSAnswer
from client.dnslib.dns import QTYPE, RCODE

@timeout(5)
def receive_response(s):
    while True:
        result, ignored = s.recvfrom(1024)
        try:
            answer = DNSAnswer(result).parse()
            return answer
        except Exception:
            print("ERROR: failed to parse the answer. Illegal response"
                  + "received. Waiting for more answers...")

if __name__ == '__main__':
    args = MyParser(sys.argv[1:])()
    domain, server = args.domain_name, args.dns_server
    server = server.strip('@')

    q = ARecordQuery(domain)
    message = q.pack()

    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    s.bind(('', 10240))
    s.sendto(message, (server, 53))

    try:
        answer = receive_response(s)
        if answer.rr:
            for r in answer.rr:
                print("%-40s\t%s\t%s" % (
                    r.get_rname(),
                    QTYPE[r.rtype],
                    r.rdata))
        else:
            print("ERROR: NXDOMAIN")
            print('Reason: %s' % (RCODE[answer.header.rcode]))

    except TimeoutError:
        print("Error: timeout querying domain '%s' on server %s" % (domain,
                                                                    server))

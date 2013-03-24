#!/usr/bin/env python
# vim: set fileencoding=utf-8 :

from __future__ import (unicode_literals, absolute_import,
                        division, print_function)

import socket
from .dnslib import DNSRecord
from .timeout import timeout

class DNSProxy(object):

    def __init__(self, upstream_addr):
        self.upstream = upstream_addr
        self.socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

    @timeout(5)
    def proxy(self, data, remote_addr):
        DNSRecord.parse(data)
        self.socket.sendto(data, (self.upstream, 53))
        data, addr = self.socket.recvfrom(1024)
        return data

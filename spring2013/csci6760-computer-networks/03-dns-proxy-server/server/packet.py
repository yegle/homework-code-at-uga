#!/usr/bin/env python
# vim: set fileencoding=utf-8 :

from __future__ import (unicode_literals, absolute_import,
                        division, print_function)

from .dnslib import DNSRecord, DNSQuestion, QTYPE

class ARecordQuery(object):

    def __init__(self, domain):
        d = DNSRecord(q=DNSQuestion(domain, QTYPE.A))
        self.data = d

    def pack(self):
        return self.data.pack()

class DNSAnswer(object):

    def __init__(self, data):
        d = DNSRecord.parse(data)
        self.answer = d

    def parse(self):
        return self.answer

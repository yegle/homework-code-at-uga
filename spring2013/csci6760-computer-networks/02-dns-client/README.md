# Computer Networks project #2

This is a DNS query client.

## Compile

No need to compile, written in Python

## Run

1. cd to the source code root directory
2. run `python nslookup.py google.com @8.8.8.8`

N.B. in order to mimic dig's behaviour, the ordering of arguments are
not relavent. It's OK to run using `python nslookup.py @8.8.8.8
google.com`

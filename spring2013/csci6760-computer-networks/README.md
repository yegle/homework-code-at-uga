# TCP echo server and test client

by Yibin Liao <tigerlyb@uga.edu> & Yuchen Ying <yegle@uga.edu>

team number 1

## Source files

We've implemented multiple servers and clients using Python.

Servers we've implemented

1. threadserver.py: implemented using Python's multithreading lib
2. forkserver.py: implemented using Python's fork function
3. geventserver.py: implemented using Python's gevent lib (a event based
   network lib)

Client we've implemented

1. forktest.py: implemented using Python's fork function
2. geventtest.py: implemented using Python's gevent lib (a event based
   network lib)
3. threadtest.py: implemented using Python's multithreading lib

## How to run the code

The code was tested under Python 2.7. We use virtualenv to create the
running environment.

Steps to prepare the environment:

1. Install virtualenv: `yum install python-virtualenv`
2. Install `libevent-devel` for installing gevent: `yum install libevent-devel`
3. Create the virtual environment: `virtualenv venv`
4. Activate virtual environment: `source venv/bin/activate`
5. Install gevent: `pip install gevent`

P.S. the environment was created on vm19, in `/home/student/venv/`
directory. If you just need to test our code, please use that
environment.

## Required System Configurations

### OpenVPN connection

In order to have more test IP addresses on client side, we uses OpenVPN
to create a virtual interface and assign more test IP addresses on it.

OpenVPN config on client side (configured on vm18):

    dev tun
    ifconfig 10.0.0.2 10.0.0.1
    secret static.key
    remote 172.17.152.19
    route 10.0.0.0 255.255.255.0

OpenVPN config on server side (configured on vm19):

    dev tun
    ifconfig 10.0.0.1 10.0.0.2
    secret static.key
    route 10.0.0.0 255.255.255.0
    log-append /var/log/openvpn.log

P.S. the `static.key` file can be generated using command: `openvpn --genkey --secret static.key`

After running openvpn server/client, we need to add more virtual IP
addresses to client side:

    ifconfig tun0:1 10.0.0.3
    ifconfig tun0:1 10.0.0.4
    ifconfig tun0:1 10.0.0.5

So we've get more available IP address for testing purpose.

### System parameters

We've changed some parameters in `/etc/sysctl.conf` file for testing.

Add the lines below to `/etc/sysctl.conf`, to both server side and
client side.

    # add more local port available for client
    net.ipv4.ip_local_port_range = 1025 65535
    # increase the maximum network connections
    net.netfilter.nf_conntrack_max = 655350

Run `sysctl -p` to make these settings effect.

### Running the code

On server side:

1. Setting the maximum open file number: `ulimit -n 1024000` (root
   permission required)
2. Activate virtualenv, follow the instructions above.
3. Run the server code using `python eventserver.py`. Use the filename
   of the server you want to test


On client side:

1. Setting the maximum open file number: `ulimit -n 1024000` (root
   permission required)
2. Activate virtualenv, follow the instructions above.
3. Run the client code using `python geventtest.py`. Use the filename of
   the client you want to test

### Testing result

We've tested in two ways

1. Test the maximum empty connections the server side can handle
2. Test the maximum throughput the server side can handle

In the first test, we have these test result:

1. threadserver.py: 29529 connections
2. forkserver.py: 6700 connections
3. eventserver.py: 220000 connections (P.S. we have to stop the test
   because it takes too long)

In the second test, we have these test result:

1. Sending 256 bytes each time
    1. threadserver.py: 134MB/s
    2. eventserver.py: 134MB/s
2. Sending 512 bytes each time
    1. threadserver.py: 273 MB/s
    2. eventserver.py: 500 MB/s

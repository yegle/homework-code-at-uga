#!/usr/bin/env bash

modprobe -r e1000
modprobe -r netdev
modprobe netdev
ifconfig E1000dev0 up
ifconfig E1000dev0 172.20.0.2
route add default gw 172.20.0.1

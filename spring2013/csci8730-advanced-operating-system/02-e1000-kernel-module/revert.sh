#!/usr/bin/env bash

modprobe -r netdev
modprobe e1000
ifconfig eth0 172.20.0.2
ifconfig eth0 up
route add default gw 172.20.0.1

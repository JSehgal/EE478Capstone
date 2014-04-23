#!/bin/sh
echo "Setting up default gateway"
/sbin/route add default gw 192.168.7.1
# echo "nameserver 8.8.8.8" >> /etc/resolv.conf
# https://github.com/JSehgal/EE478Capstone.git
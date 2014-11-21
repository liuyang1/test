#! /usr/bin/env python

import mcast
mcastAddr="239.136.94.241"
localAddr="192.168.1.111"

sock=mcast.McastSocket(local_port=5000,reuse=1)
sock.mcast_add(mcastAddr,localAddr)

while 1:
    print sock.recv(10240)

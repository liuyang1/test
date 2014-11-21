#! /usr/bin/env python
import socket
import struct

MCAST_GRP="224.0.0.1"
MCAST_PORT=5000

sock=socket.socket(socket.AF_INET,socket.SOCK_DGRAM,socket.IPPROTO_UDP)
sock.setsockopt(socket.SOL_SOCKET,socket.SO_REUSEADDR,1)
sock.bind(('',MCAST_PORT))
mreq=struct.pack("4sl",socket.inet_aton(MCAST_GRP),socket.INADDR_ANY)

sock.setsockopt(socket.IPPROTO_IP,socket.IP_ADD_MEMBERSHIP,mreq)

while 1:
    print sock.recv(10240)


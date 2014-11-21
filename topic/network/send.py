#! /usr/bin/env python
import socket

MCAST_GRP="239.136.94.241"
MCAST_PORT=5000

sock=socket.socket(socket.AF_INET,socket.SOCK_DGRAM,socket.IPPROTO_UDP)
sock.setsockopt(socket.IPPROTO_IP,socket.IP_MULTICAST_TTL,2)

while 1:
    try:
        s=raw_input(">>")
    except EOFError:
        break
    except KeyboardInterrupt:
        break
    sock.sendto(s,(MCAST_GRP,MCAST_PORT))

#! /usr/bin/env python
from socket import *
# config content
forward_addr=('222.195.78.248',1235)
lo_addr=('127.0.0.1',1235)
# end of config
sock=socket(AF_INET,SOCK_DGRAM)
sock.sendto("access",forward_addr)
lo=socket(AF_INET,SOCK_DGRAM)
pktlen=2048
while 1:
	data,addr = sock.recvfrom(pktlen)
	lo.sendto(data,lo_addr)
sock.close()
lo.close()

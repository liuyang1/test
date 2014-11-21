#! /usr/bin/env python
from socket import *
import AT
host = '114.214.194.185'
port = 10001
sock = socket(AF_INET,SOCK_DGRAM)
sock.bind((host,port))
sock.sendto("access",('222.195.78.248',1235))

lo=socket(AF_INET,SOCK_DGRAM)
lo.bind(('127.0.0.1',1234))

while 1:
	data,addr = sock.recvfrom(2048)
	print addr,len(data)
	lo.sendto(data,('127.0.0.1',1235))

sock.close()
lo.close()

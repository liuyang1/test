#! /usr/bin/env python
from socket import *
host = 'localhost'
port = 1234
addr=(host,port)
sock = socket(AF_INET,SOCK_DGRAM)
while 1:
	str=raw_input(">>")
	sock.sendto(str,addr)
	data = sock.recvfrom(1024)
	print data
	data = sock.recvfrom(1024)
	print data
sock.close()

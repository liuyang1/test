#! /usr/bin/env python
from socket import *
host = 'localhost'
port = 10001
sock = socket(AF_INET,SOCK_DGRAM)
sock.bind((host,port))
sock.sendto("access",(host,8081))

while 1:
	data = sock.recvfrom(1024)
	print data
sock.close()

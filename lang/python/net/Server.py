#! /usr/bin/env python
from socket import *
host = 'localhost'
port = 8081
sock = socket(AF_INET,SOCK_DGRAM)
sock.bind((host,port))
while 1:
	data = sock.recvfrom(1024)
	print data
sock.close()

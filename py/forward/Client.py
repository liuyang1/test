#! /usr/bin/env python
from socket import *
host = 'localhost'
port = 8080
addr=(host,port)
sock = socket(AF_INET,SOCK_DGRAM)
while 1:
	try:
		str=raw_input(">>")
		sock.sendto(str,addr)
	except:
		break
sock.close()

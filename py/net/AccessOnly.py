#! /usr/bin/env python
from socket import *
from time import sleep
import AT
host = 'localhost'
dstport = 10001
sock = socket(AF_INET,SOCK_DGRAM)
dstaddr=(host,dstport)
while 1:
	dsttext=AT.addr2text(dstaddr)
	print dsttext
	sock.sendto(dsttext,(host,8081))
	sleep(10)
sock.close()

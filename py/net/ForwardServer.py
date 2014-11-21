#! /usr/bin/env python
from socket import *
import thread
from time import sleep
import AT
host = 'localhost'
inport = 8080
outport = 8081
insock = socket(AF_INET,SOCK_DGRAM)
insock.bind((host,inport))
outsock = socket(AF_INET,SOCK_DGRAM)
outsock.bind((host,outport))

_addrlist={}

def forward():
	while 1:
		data,inaddr = insock.recvfrom(1024)
		print data,inaddr
		forwardlist(data)
	insock.close()
	outsock.close()

def access():
	while 1:
		data,accaddr = outsock.recvfrom(1024)
		dstaddr = AT.text2addr(data)		
		print dstaddr
		addAddrlist(dstaddr)
	outsock.close()

def forwardlist(data):
	print _addrlist
	for addr,ttl in _addrlist.iteritems():
		print addr
		outsock.sendto(data,addr)

def addAddrlist(addr):
	_addrlist[addr]=0

def inctime():
	global _addrlist
	_addrlist={k:v+1 for k,v in _addrlist.iteritems()}

def deltimeout():
	global _addrlist
	_addrlist={k:v for k,v in _addrlist.iteritems() if v <= 20}

def timeout():
	while 1:
		sleep(1)
		deltimeout()
		inctime()
		print "link num:",len(_addrlist)

if __name__=="__main__":
	thread.start_new(access,())
	thread.start_new(timeout,())
	forward()

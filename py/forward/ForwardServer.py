#! /usr/bin/env python2.7
from socket import *
import thread
from time import sleep
# config content
host='222.195.78.248'
inport=1234
outport=1235
# end of config content
insock=socket(AF_INET,SOCK_DGRAM)
insock.bind((host,inport))
outsock=socket(AF_INET,SOCK_DGRAM)
outsock.bind((host,outport))
_addrlist=[]
pktlen=2048
def forward():
	while 1:
		data,inaddr = insock.recvfrom(pktlen)
		forwardlist(data)
	insock.close()
	outsock.close()

def access():
	while 1:
		data,accaddr = outsock.recvfrom(pktlen)
		addAddrlist(accaddr)

def forwardlist(data):
	for addr in _addrlist:
		outsock.sendto(data,addr)

def addAddrlist(addr):
	print "add ",addr
	_addrlist.append(addr)

if __name__=="__main__":
	thread.start_new(access,())
	forward()

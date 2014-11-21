#! /usr/bin/env python
#
#	this tool help to recv pkt from specfic port using socket
#	it can output info realtime to screen or log file
#	and it count number of packets per second,so you can know program process dynamic.
#	finally,it can exit auto by 10 seconds timeout.
#	you can go away to do sth and don't need to exit roughly by using Ctrl+D
#
#	liuyang1,2012-04-03 21:47:30,winlab
import socket
from dummy_threading import *
import threading
import sys
import time
filename="/tmp/stream.ts"
port=5000
buflen=2048
ttl=3

cnt=0
lastcnt=0
bytecnt=0.0
lastbytecnt=0.0
cnttime=0.0
def dispcnt():
	global cnt,lastcnt,bytecnt,lastbytecnt,cnttime
	cnttime=cnttime+1
	print "[%s] recv[1s]: %4.0f B %4d pkts | total: %4.0f B/s %4.0f pkts/s %6.0f B %6d pkts\r"%(time.ctime(),bytecnt-lastbytecnt,cnt-lastcnt,bytecnt/cnttime,cnt/cnttime,bytecnt,cnt),
	lastcnt=cnt
	lastbytecnt=bytecnt

class Log(object):
	def __init__(self):
		#self.f=file("log.txt","w")
		sys.stdout=self
	def write(self,data):
		#self.f.write(data)
		sys.__stdout__.write(data)
		sys.__stdout__.flush()

class TimerThread(threading.Thread):
	def __init__(self,name='TimerThread'):
		self._stopevent=threading.Event()
		self._sleepperiod=1.0
		threading.Thread.__init__(self,name=name)
	def run(self):
		while not self._stopevent.isSet():
			self._stopevent.wait(self._sleepperiod)
			dispcnt()
	def join(self,time=None):
		self._stopevent.set()
		threading.Thread.join(self)

if __name__=="__main__":
	log=Log()
	fp=open(filename,"wb")
	print "[%s] open file <%s>"%(time.ctime(),filename)

	sock=socket.socket(socket.AF_INET,socket.SOCK_DGRAM)
	sock.setsockopt(socket.SOL_SOCKET,socket.SO_REUSEADDR,1)
	ret=sock.bind(("",port))
	print ret
	print "[%s] listning on port %d"%(time.ctime(),port)
	lasttime=time.time()
	timer=TimerThread()
	timer.start()
	while 1:
		sock.settimeout(ttl)
		try:
			data,addr=sock.recvfrom(buflen)
			fp.write(data)
			cnt=cnt+1
			#bytecnt=bytecnt+len(data)/1024.0
			bytecnt=bytecnt+len(data)
		except socket.timeout:
			timer.join()
			print "\n[%s] time up!"%time.ctime()
			break
	fp.close()
	sock.close()
	print "[%s] stop"%time.ctime()

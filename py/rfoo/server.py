#! /bin/python 
import rfoo

class MyHandler(rfoo.BaseHandler):
	def echo(self,str):
		return "hi,"+str
	def add(self,x,y):
		return x+y

rfoo.InetServer(MyHandler).start(port=5000)

#! /bin/python 
import rfoo,sys
try:
	c=rfoo.InetConnection().connect(port=5000)
except:
	print 'connect error'
	sys.exit(-1)
print rfoo.Proxy(c).echo('liuy')
print rfoo.Proxy(c).add(2,3)
print rfoo.Proxy(c).dumps()

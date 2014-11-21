#! /bin/python 
from xmlrpclib import ServerProxy,Error
server=ServerProxy("http://betty.userland.com")
print server
try:
	print server.examples.getStateName(41)
except Error,v:
	print "ERROR",v

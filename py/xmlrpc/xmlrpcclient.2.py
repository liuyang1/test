#! /bin/python 
from xmlrpclib import ServerProxy,Error
server=ServerProxy("http://localhost:8000")
print server
try:
	print server.pow(2,3)
	print server.add(2,3)
	print server.div(5,2)
	print server.system.listMethods()
except Error,v:
	print "ERROR",v

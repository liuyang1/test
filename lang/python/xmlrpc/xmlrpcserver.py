#! /bin/python 
from SimpleXMLRPCServer import SimpleXMLRPCServer
from SimpleXMLRPCServer import SimpleXMLRPCRequestHandler

class RequestHandler(SimpleXMLRPCRequestHandler):
	rpc_paths=('/RPC2',)

server=SimpleXMLRPCServer(("",8000),requestHandler=RequestHandler)
server.register_introspection_functions()
server.register_function(pow)

def adder_funtion(x,y):
	return x+y
server.register_function(adder_funtion,'add')

class MyFuncs:
	def div(self,x,y):
		return x//y
server.register_instance(MyFuncs())

server.serve_forever()

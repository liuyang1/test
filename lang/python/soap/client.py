from suds.client import Client

test = Client('http://localhost:7789/SOAP/?wsdl')

print "wait for reply"
ret = test.service.say_hello('liuy',5)
print ret
print test.last_received

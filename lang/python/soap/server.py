import soaplib
from soaplib.core.server import wsgi
from soaplib.core.service import DefinitionBase
from soaplib.core.service import soap
from soaplib.core.model.clazz import Array
from soaplib.core.model.clazz import ClassModel
from soaplib.core.model.primitive import Integer, String
from wsgiref.simple_server import make_server
import sys


class HelloWorldService(DefinitionBase):
# this is _returns
    @soap(String, Integer, _returns=Array(String))
    def say_hello(self, name,times):
        return ['Hello, %s' % name, "abc"]


def startSvc():
    soap_application = soaplib.core.Application([HelloWorldService], 'tns')
    wsgi_application = wsgi.Application(soap_application)
    print "make svc"
    server = make_server('localhost', 7789, wsgi_application)
    print "start svc..."
    server.serve_forever()


if __name__ == "__main__":
    startSvc()

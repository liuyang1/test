#! /usr/bin/env python
from xmlrpclib import Server
server=Server('http://user:123@127.0.0.1:9001')

info=server.supervisor.getProcessInfo('test')
if 'STOPPED'==info['statname']:
	print 'server is stopped,we start it!'
	server.supervisor.startProcess('test')
elif 'RUNNING'==info['statname']:
	print 'server is running,web stop id!'
	server.supervisor.stopProcess('test')

print 'test process state is',server.supervisor.getProcessInfo('test')

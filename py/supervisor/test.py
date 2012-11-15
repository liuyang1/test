#! /bin/python 
import web
urls=('/(.*)','hello')
app=web.application(urls,globals())

class Hello:
	def GET(self,name):
		return 'hello: '+name+'!'
app=run()

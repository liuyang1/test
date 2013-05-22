#! /usr/bin/env python
import os,sys
#abspath = os.path.dirname(__file__)
#print abspath
#sys.path.append(abspath)
#os.chdir(abspath)
print sys.path
import web
urls = ("/.*","hello")
app=web.application(urls,globals())

class hello:
	def GET(sef):
		return "hello,world!"

if __name__=="__main__":
	app.run()

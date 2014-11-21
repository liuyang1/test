#! /usr/bin/env python
import os
path="/home/liuy/down"
filelist=os.listdir(path)
print filelist
for f in filelist:
	print "stat ",os.stat(path+"/"+f)
	print "size ",os.path.getsize(path+"/"+f)
	print "mtime",os.path.getmtime(path+"/"+f)

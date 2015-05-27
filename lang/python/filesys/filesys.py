#! /usr/bin/env python
import os
import time
path = "/home/liuyang/down"
filelist = os.listdir(path)
print filelist
for f in filelist:
    stat = os.stat(path + "/" + f)
    print "stat ", stat
    print "size ", os.path.getsize(path + "/" + f)
    mtime = os.path.getmtime(path + "/" + f)
    print "mtime", mtime
    print "ctime", time.ctime(mtime)

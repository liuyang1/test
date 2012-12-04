#! /usr/bin/env python
"""
read and write test for Storage System
liuyang1,mtime: 2012-12-04 13:18:11 
"""

import os
from random import randint
import log

g_path="/home/liuy/video/"
g_logger=log.initLogging("RWT")

def copy(filename):
	cmd="cp "+g_path+filename+" /tmp/"
	os.popen(cmd)
	g_logger.info(cmd)
	return "/tmp/"+os.path.basename(filename)

def remove(filename):
	cmd="rm "+filename
	os.popen(cmd)
	g_logger.info(cmd)
	return

def rand(filelist):
	return filelist[randint(0,len(filelist)-1)%len(filelist)]

def getFileList(filepath):
	return os.listdir(filepath)

def ntimescp(filelist,n=1):
	for i in range(n):
		filename=copy(rand(filelist))
		remove(filename)
	
def main():
	fl=getFileList(g_path)
	ntimescp(fl);

if __name__=="__main__":
	main()

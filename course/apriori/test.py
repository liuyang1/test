#! /usr/bin/python
import sys
var=1
def varplus():
	global var
	var=var+1
	sys.exit()

if __name__=="__main__":
	print var
	varplus()
	print var
	varplus()
	print var

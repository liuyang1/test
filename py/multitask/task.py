#! /usr/bin/env python
import multitask
import time
def crtn1():
	for i in range(3):
		print "crtn1"
		yield i#must have this,becuase multitask func must be a generator

def crtn2():
	for i in range(3):
		print "crtn2"
		yield i

def input():
	while 1:
		line=raw_input(">")
		print line
		yield line

multitask.add(crtn1())
multitask.add(crtn2())
multitask.add(input())

multitask.run()

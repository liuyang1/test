#!/bin/python
def printEle(lst):
	for i in lst:
		if i==lst:
			raise Exception('loop')
		elif isinstance(i,list):
			printEle(i)
		else:
			print i
def test1():
	a=[1,2,[3,4,[5,6],7,],8,9]
	print a
	printEle(a)

def test2():
	b=[0]
	a=[b]
	b[0]=a
	print b
	printEle(b)

if __name__=="__main__":
	test2()

#! /usr/bin/python
import random
import sys
class Log(object):
	def __init__(self):
		self.f=file("out.txt","w")
		sys.stdout=self
	def write(self,data):
		self.f.write(data)
		sys.__stdout__.write(data)
		sys.__stdout__.flush()

def hashFunc(data,n):
	return data

def testHashTable(n,itemnum):
	ht=[0]*n
	for i in range(itemnum):
		data=random.randint(0,n-1)
		index=hashFunc(data,n)
		ht[index]=ht[index]+1
	sum=0
	for cnt in ht:
		sum=sum+(cnt*(cnt+1))/2
#	k=sum/(itemnum+0.0)
#	print n,itemnum,sum,k
	return sum

if __name__=="__main__":
	Log=Log()
	for i in range(15):
		cnt=0
		for j in range(100):
			cnt=cnt+testHashTable(1024,2**i)/(2**i+0.0)
		cnt/=(100+0.0)
		print 1024,2**i,cnt

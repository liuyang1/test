#! /usr/bin/python
# 2012-04-16 00:08:12 liuyang1@dorm
# least cost shopping
# usage: shopping.py
infile1="input.txt"
infile2="offer.txt"
outfile="output.txt"

def shopping(lst,offer):
	pass
	
def main(inFile1,inFile2,outFile):
	b=int(inFile1.readline())
	lst=[]
	for i in range(b):
		record=inFile1.readline()
		record=record.split(" ")
		record=[int(item) for item in record]
		lst.append(record)
	s=int(inFile2.readline())
	offer=[]
	for i in range(s):
		record=inFile2.readline()
		record=record.split(" ")
		record=[int(item) for item in record]
		offer.append(record)
	print s
	print offer
	shopping(lst,offer)

if __name__=="__main__":
	inFile1=open(infile1,"r")
	inFile2=open(infile2,"r")
	outFile=open(outfile,"w")
	main(inFile1,inFile2,outFile)

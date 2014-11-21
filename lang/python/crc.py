#! /usr/bin/env python
import zlib 
def crc(fileName):
	prev = 0
	for eachLine in open(fileName,"rb"):
		prev = zlib.crc32(eachLine, prev)
	return "%X"%(prev & 0xFFFFFFFF)

testfile="testfile"
he=crc(testfile)
print he

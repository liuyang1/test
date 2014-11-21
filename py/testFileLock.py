#! /bin/python 
import fcntl
import time
filename='test.txt'

fp = open(filename,'w')
print 'open %s write mode'%(filename)

fcntl.flock(fp,fcntl.LOCK_EX) ##阻塞执行,也就是如果该文件被锁定,则程序阻塞在此
print 'file %s locking...'%(filename)
time.sleep(20)
fcntl.flock(fp,fcntl.LOCK_UN)
fp.close()

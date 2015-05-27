import time
import os.path

def getMTime(f):
    return os.path.getmtime(f)


def showMTime(t):
    return time.ctime(t)


f = "testfile.py"
import testfile

mtime0 = getMTime(f)
print "init"
print showMTime(mtime0)
print testfile.v
while 1:
    time.sleep(3)
    try:
        mtime = getMTime(f)
    except OSError:
        print "access %s fail" % f
        continue
    if mtime != mtime0:
        print "update"
        mtime0 = mtime
        print showMTime(mtime0)
        print "reload"
        reload(testfile)
        print testfile.v
    else:
        print "not change"
        print testfile.v

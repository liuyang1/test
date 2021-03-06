#!/bin/python
import os
import signal
from fcntl import *

FNAME = "."


def handler(signum, frame):
    print "File %s modified" % (FNAME,),
    print signum, frame

signal.signal(signal.SIGIO, handler)
fd = os.open(FNAME, os.O_RDONLY)
fcntl(fd, F_SETSIG, 0)
fcntl(fd, F_NOTIFY, DN_MODIFY | DN_CREATE | DN_MULTISHOT)
while True:
    pass

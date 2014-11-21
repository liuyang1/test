#! /usr/bin/env python
import pysftp

srv=pysftp.Connection("222.195.78.248",username="ustc",password="225225")
ret=srv.execute('ls')
print ret

srv.execute("get liuy/hash.py /home/liuy/test")

srv.close()

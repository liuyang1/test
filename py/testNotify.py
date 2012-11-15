#!/bin/python
# http://timgolden.me.uk/python/win32_how_do_i/watch_directory_for_changes.html
# GOOD
# easy to write and to understand
# Use only std modules
# Works across any platform
# Run in respectable time for small-medium number of files
# BAD
# Doesn't scale well(although it's not too bad if you don't expect many changes)
# Doesn't account for updated files(although you could get fancy with os.stat)
# MORE
# other windows method see the Page
import os,time

watchPath="."

print 'Access time:  ',time.ctime(os.path.getatime(watchPath))
print 'Modified time:',time.ctime(os.path.getctime(watchPath))
print 'Change time:  ',time.ctime(os.path.getmtime(watchPath))
print 'size:		 ',os.path.getsize(__file__)
before=dict([(f,None) for f in os.listdir(watchPath)])

print before
while 1:
	time.sleep(1)
	after=dict([(f,None) for f in os.listdir(watchPath)])
	added = [f for f in after if not f in before]
	removed = [f for f in before if not f in after]
	if added:
		print "Added: ",", ".join(added)
	if removed:
		print "Removed:",", ".join(removed)
	before=after

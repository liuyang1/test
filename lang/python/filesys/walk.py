import os

d = "/tmp"
for root, dirs, files in os.walk(d):
    print "root: %s" % (root)
    print "dirs: %s" % (dirs)
    print "files: %s" % (files)

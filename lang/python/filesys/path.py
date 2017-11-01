from os.path import *


def testDir(d):
    print basename(d)
    print dirname(d)


d = "/a/b/c"
testDir(d)
d = "/a/b/c/"
testDir(d)

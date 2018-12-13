import sys


fn = sys.argv[1]
print fn

with open(sys.argv[1], "rb") as fp:
    print fp.read()
    for b in fp.read():
        print ord(b)

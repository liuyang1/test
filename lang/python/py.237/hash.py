#! /usr/bin/env python
import hashlib
objfile="testfile"
sha=hashlib.sha1()

with open(objfile,"rb") as f:
	for chunk in iter(lambda: f.read(8192),b''):
		sha.update(chunk)
print sha.hexdigest()

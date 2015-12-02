#! /usr/bin/env python3

"""
test timeout feature in subprocess library

Ref: https://docs.python.org/3/library/subprocess.html#subprocess.check_output
"""
import subprocess


cmd = "ls"

ret = subprocess.check_output(cmd, shell=True, timeout=1)
print(ret)

cmd = "sleep 2 && ls"
try:
    ret = subprocess.check_output(cmd, shell=True, timeout=1)
    print(ret)
except subprocess.TimeoutExpired:
    print("timeout when check_output")

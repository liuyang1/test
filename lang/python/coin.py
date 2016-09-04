#! /usr/bin/env python2
"""
American have 54 states, one states depoly one kind of quaters.
Assume we could get any kind of coin with same probablity,
       we want to collect all kinds of quaters.
How many quaters need to get in average?

246.5
"""
import random

N = 54

def getCoin():
    """ [0, N -1] """
    return random.randint(0, N - 1)

def getFullSet():
    cnt = 0
    st = set()
    while True:
        st.add(getCoin())
        cnt += 1
        if len(st) == N: # get all kinds of coins
            return cnt
        

def avgNum():
    times = 10000
    lst = [getFullSet() for i in range(times)]
    return sum(lst) / (len(lst) + 0.0)

print avgNum()

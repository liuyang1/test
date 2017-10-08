#! /usr/bin/env python3


def gen():
    i = 1
    while True:
        yield i
        i *= 2


g = gen()
for i in range(10):
    print(next(g))

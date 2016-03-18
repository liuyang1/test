#! /usr/bin/env python2
"""
output snake shape char
"""
dot = '.'
sharp = '#'


def snake(n, m):
    s0 = sharp * m
    dots = dot * (m - 1)
    s1 = dots + sharp
    s3 = sharp + dots
    for i in xrange(n):
        if i % 2 == 0:
            print s0
        elif i % 4 == 1:
            print s1
        else:
            print s3


def main():
    line = raw_input()
    d = [int(i) for i in line.split()]
    snake(d[0], d[1])


if __name__ == "__main__":
    main()

#! /usr/bin/env python


def count(lst):
    """
    >>> count([1,2,3])
    0
    >>> count([1,3,2])
    1
    """
    cnt = 0
    max=lst[0]
    for i in range(1,len(lst)):
        if lst[i]<max:
            cnt += 1
        else:
            max=lst[i]
    return cnt


def case(seq):
    n = int(raw_input())
    lst = []
    for i in range(n):
        lst.append(raw_input())
    print "Case #%d: %d"%(seq,count(lst))

if __name__ == "__main__":
    n = int(raw_input())
    for i in range(n):
        case(i+1)

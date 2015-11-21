#! /usr/bin/env python2
"""
uniq on list, but keep order

ref: http://www.peterbe.com/plog/uniqifiers-benchmark
"""


def uniq(seq):
    seen = set()
    seen_add = seen.add
    # if x not in seen, add it.
    # seen.add(x) return None, so when seen.add it, generate list
    return [x for x in seq if not (x in seen or seen_add(x))]

lst = [0, 4, 1, 2, 3, 4, 3, 1]
print lst
print uniq(lst)

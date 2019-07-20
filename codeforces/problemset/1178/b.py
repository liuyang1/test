from itertools import *
def RLE(l):
    return [(name, len(list(group))) for name, group in groupby(l)]


def vv2w(l):
    return [(c, n) if c == 'o' else ('w', n - 1) for (c, n) in l]

s = raw_input()
en = vv2w(RLE(s))

wcnt = sum([n for (c, n) in en if c == 'w'])

pcnt = 0
s = 0
for (c, n) in en:
    if c == 'o':
        s += n * pcnt * (wcnt - pcnt)
    elif c == 'w':
        pcnt += n
print(s)

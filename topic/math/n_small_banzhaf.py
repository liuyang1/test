from math import ceil, floor
from scipy.special import comb

def comba(m, n):
    return comb(m, n, exact=True)

def C(m, n):
    return comb(m, n, exact=False)

"""
s = 13 * comba(20, 13) + 12 * comba(20, 12) + 9*comba(20, 9) + 8*comba(20, 8)
b = comba(20, 12) + comba(20, 8)
a = sum(comba(20, i) for i in range(9, 12 + 1)) + sum(comba(20, i) for i in range(8, 11 + 1))

ss = s + a + b
print(s, b, a)

print(s/ss, s/ss/20.)
print(a/ss)
print(b/ss)
"""


def banzhafN(N=20):
    def low(p):
        return int(ceil(p/0.77*N))
    def high(p):
        return int(floor(p/0.77*N))

    ks = [int(ceil(p/0.77*N)) for p in [0.5, 0.45, 0.32, 0.27]]
    #print(ks)
    s = [k * C(N, k) for k in ks]
    #print(sum(s))

    ks = range(low(0.45), high(0.5) + 1)
    #print(ks)
    b = [C(N, k) for k in ks]
    ks = range(low(0.27), high(0.33) + 1)
    #print(ks)
    b.extend([C(N, k) for k in ks])
    #print(b)
    #print(sum(b))

    ks = range(low(0.32), high(0.5) + 1)
    #print(ks)
    a = [C(N, k) for k in ks]
    ks = range(low(0.27), high(0.45) + 1)
    #print(ks)
    a.extend([C(N, k) for k in ks])
    #print(sum(a))

    s = sum(s)
    b = sum(b)
    a = sum(a)
    ttl = s + b + a
    #print(s, b, a)
    #print(s/ttl, s/ttl/N, b/ttl, a/ttl)
    return (s/ttl, s/ttl/N, b/ttl, a/ttl)

print(banzhafN(20))

for N in [1, 2, 5, 10, 20, 50, 100, 200, 500, 700, 800, 1000]:
    print(N, banzhafN(N))

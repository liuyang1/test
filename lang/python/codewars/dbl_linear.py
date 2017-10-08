#! /usr/bin/python3
import heapq


def heapStInsert(q, st, x):
    if x not in st:
        heapq.heappush(q, x)
        st.add(x)


def seq():
    q = [1]
    st = set(q)
    while True:
        r = heapq.heappop(q)
        yield r
        heapStInsert(q, st, 2 * r + 1)
        heapStInsert(q, st, 3 * r + 1)


def dbl_linear(n):
    q = seq()
    for i in range(n + 1):
        r = next(q)
    return r


def test():
    print(dbl_linear(0))
    print(dbl_linear(1))
    print(dbl_linear(2))
    print(dbl_linear(21))
    # print(dbl_linear(20000))

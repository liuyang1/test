#! /usr/bin/env python3
from collections import deque


def dbl_linear(n):
    h = 1
    q2, q3 = deque([]), deque([])
    for _ in range(n):
        q2.append(2 * h + 1)
        q3.append(3 * h + 1)
        h = min(q2[0], q3[0])
        # - auto remove duplicate elements in this step
        # - de-queue is O(1) for `append` and `popleft` operation
        if h == q2[0]:
            q2.popleft()
        if h == q3[0]:
            q3.popleft()
    return h


def test(dbl_linear):
    print(dbl_linear(0))
    print(dbl_linear(1))
    print(dbl_linear(2))
    print(dbl_linear(21))
    print(dbl_linear(20000))


test(dbl_linear)

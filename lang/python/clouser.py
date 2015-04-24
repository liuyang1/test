#! /usr/bin/env python3
def f(x):
    def g(y):
        # NEED THIS
        nonlocal x
        x = x - y
        return x
    return g


g0 = f(100)
ans0 = g0(42)
print(ans0)
g1 = f(200)
ans1 = g1(42)
print(ans1)

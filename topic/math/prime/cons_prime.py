"""
- 3 is prime
- 13 is prime
- 113 is iprime
- 2113 is prime

find the largest number, than all its suffix are prime.

357 686 312 646 216 567 629 137

This code cannot resolve this issue, due to test for divisibility are so slow for large number.
We resolve this problem with Mathematica.
It's PrimeQ function use Miller-Rabin strong psedoprime test.
"""

def isPrime(x):
    # if x == 1:
    #     return False
    # if x % 2 == 0:
    #     return False
    i = 3
    while i * i <= x:
        if x % i == 0:
            return False
        i = i + 2
    return True

xs = [3, 7]


def next(t, xs):
    lst = []
    for i in xs:
        for j in range(1, 10):
            n = j * (10 ** t) + i
            if isPrime(n):
                lst.append(n)
    return t + 1, lst

t = 1
while True:
    t, xs = next(t, xs)
    if len(xs) == 0:
        break
    else:
        print len(xs), xs[0:10]

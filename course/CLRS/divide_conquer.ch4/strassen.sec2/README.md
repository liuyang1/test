# Strassen algo

When matrix's size is greater than [4096, 4096], then Strassen algo is better than naive algo.

# Exercise

## 4.2.1

skip

## 4.2.2

check mat_mul.c

## 4.2.3

padding with zeros

## 4.2.4 How to improve 3x3 matrix calc?

The naive method, use 3x3x3 = 27 times.

Actually, we need 33 times multiplication.

## 4.2.5 V.Pan

check pan.py

The best methold is 2nd.

all are better than Strassen algo

## 4.2.6 (kn,n) x (n,kn)

(kn,n) x (n,kn)

This result is (kn, kn). It need k^2 times (n, n) multiplication

(n,nk) * (kn, n)

The result is (n, n). It need k times (n, n) matrix multiplication


## 4.2.7 3-multiplication to evaluate (a+bi)(c+di)

(a+bi) => | a -b |
          | b  a |

(isomorphism)

so we could use Strassen's algo

(a+bi)(c+di) = | a -b |   |c -d |
               | b  a | X |d  c |

    P1 = (a + b)(c + d)
    P2 = ac
    P3 = bd
    =>
    ac - bd = P2 - P3
    ad + bc = (ac + bc + ad + bd) - ac - bd
            = P1 - P2 - P3

check complex.c

### extension reading

This come from Karatsuba's algo/Guass.
ref: https://en.wikipedia.org/wiki/Karatsuba_algorithm

We also could write mulplication algo with this Karatsuba's algo.

    (aB+b)(cB+d)
       = acB2 + (bc+ad)B + bd

    P1 = ac
    P2 = bd
    P3 = (a + b)(c + d) - ac - bd
       = (b - a)(c - d) + ac + bd
    This will help to avoid overflow issue

#### Toom-Cook algo

>>>  This part just extension with Karatsuba algo.

It's kind of generalized Karatsuba algo. It split number to 3 sections but not only 2 sections.
So we could improve 5/9 instead of 3/4.

    5/9/(3/4)=20/27

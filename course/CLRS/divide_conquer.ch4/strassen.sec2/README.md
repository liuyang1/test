# Exercise

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

It's kind of generalized Karatsuba algo. It split number to 3 sections but not only 2 sections.
So we could improve 5/9 instead of 3/4.

    5/9/(3/4)=20/27

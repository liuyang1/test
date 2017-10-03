module InfiniteList where

next f x = (f x, f)

-- How to generate infinite list
ev nextf f x = v: ev nextf g v
             where (v, g) = nextf f x

zero = 0
inc x = x + 1
nat = ev next inc zero
-- different function
evens = ev next (inc . inc) zero

one = 1
dbl x = 2 * x
-- different basic element (unit and operaiton) of sequence
pow2' = ev next dbl one

zero2 = (1, 1)
inc2 (a, b) = (b, a + b)
fib = map fst $ ev next inc2 zero2

-- different next function
-- 1 <= inc 0
-- 3 <= inc inc 1
-- 7 <= inc2 inc2 3
-- 15 <= inc3 inc3 7
-- ...
skip f x = (f x, f . f)
pow2n1 = ev skip inc zero

main = do
        print $ take 10 nat
        print $ take 10 evens
        print $ take 10 fib
        print $ take 10 pow2'
        print $ take 10 pow2n1

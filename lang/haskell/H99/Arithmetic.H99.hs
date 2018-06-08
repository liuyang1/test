import Data.List (group)
import Data.Function (on)
import Control.Arrow ((&&&))

isPrime 1 = False
isPrime n = not $ any (\x -> n `mod` x == 0) $ takeWhile (\x -> x * x <= n) [2..]

myGCD a b
  | a == b = a
  | a < b = myGCD b a
  | otherwise = myGCD (a - b) b

coprime a b = myGCD a b == 1

-- 34
totient n = length $ filter (coprime n) [1..(n - 1)]

-- 35 prime factors
go p n
  | 1 == n = []
  | p * p > n = [n]
  | (d, 0) <- n `divMod` p = p: go 2 d
  | otherwise = go (p + 1) n

primeFactors = go 2

-- 36 prime factors multiple
primeFactorsMult = map (head &&& length) . group . go 2

-- 37
totient1 = product . map (\(p, m) -> (p - 1) * p ^ (m - 1)) . primeFactorsMult

-- 38
-- totient1 is faster than totient
-- totient: O(n * n)
-- totient1: O(n)

-- 39
primesR a b = filter isPrime [a..b]

-- 40
-- goldbach's conjecture
goldbach n = filter (uncurry ((&&) `on` isPrime)) $ map (\x -> (x, n - x)) [2..(div n 2)]

goldbach1 = head . goldbach

goldbachList a b = map goldbach1 $ filter even [a..b]
goldbachList1 a b t = filter (\(a, _) -> a > t) $ goldbachList a b

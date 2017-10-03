module Emirps where

import Prime

-- Emirps is a prime sequence, whose reversed num is a different prime.
-- ref: https://oeis.org/A006567

rev ::  Integer -> Integer
rev = read . reverse . show

inOrdered x [] = False
inOrdered x (y:ys) = case compare x y of
                       LT -> False
                       EQ -> True
                       GT -> inOrdered x ys

emirps = filter (\x -> let r = rev x in r /= x && r `inOrdered` primes) primes

stat [] = [0, 0, 0]
stat xs = map ($ xs) [toInteger . length, last, sum]
-- applicative style
-- stat xs = [toInteger . length, last, sum] <*> pure xs

findEmirps n = stat $ takeWhile (<= n) emirps

main = print $ findEmirps 7523

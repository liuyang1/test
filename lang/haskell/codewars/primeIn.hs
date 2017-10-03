module Codewars.Kata.PrFactors where

import Data.List

primes = sieve [2..]
    where sieve (p:xs) = p : sieve [x | x <- xs, x `mod` p /= 0]
primeFs xs n p
  | n == 1 = xs
  | rem n p == 0 = primeFs (p:xs) (n `div` p) p
  | otherwise = primeFs xs n (p + 1)
prime_factors :: Integer -> String
prime_factors 1 = "(1)"
prime_factors n = concat . reverse . map fn . group $ primeFs [] n 2
  where fn x = let len = length x
                   n = head x
                in if len /= 1 then "(" ++ show n ++ "**" ++ show len ++ ")"
                               else "(" ++ show n ++ ")"

main = print $ prime_factors 1232347587

module Codewars.Kata.SumByFactors where
import Data.List

pfactorh n p fs
  | n == 1 = fs
  | n `mod` p == 0 = pfactorh (n `div` p) p (p:fs)
  | p * p > n = pfactorh n n fs
  | otherwise = pfactorh n (p + 1) fs
pfactor n = pfactorh (abs n) 2 []
sumOfDivided :: [Integer] -> [(Integer, Integer)]
sumOfDivided ns = map (\p -> (p, sum $ filter (\x -> x `mod` p == 0) ns)) ps
  where ps = sort . nub . concat $ map pfactor ns

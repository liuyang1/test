module Codewars.Kata.RemovNB where
import Data.List

fn s n a
  | r == 0 && b <= n = [(a, b)]
  | otherwise = []
    where d = s - a
          r = d `mod` (a + 1)
          b = d `div` (a + 1)
removNb :: Integer-> [(Integer, Integer)]
removNb n = concatMap (fn s n) [1..n]
  where s = n * (n + 1) `div` 2

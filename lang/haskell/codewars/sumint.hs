module Codewars.Kata.Summing where

f :: Integer -> Integer -> Integer
f n m
  | n >= m = n `div` m * f (m - 1) m + f (rem n m) m
  | otherwise = foldl (+) 0 [0..n]

module Codewars.Kata.Suite1 where

go :: Integer -> Integer -> Double -> Double
go n x acc
  | n == x = acc
  | otherwise = go n (x + 1) (acc / (fromInteger x + 1)  + 1)

-- going :: Integer -> Double
going n = (fromInteger $ floor $ (go n 1 1) * (10^6)) / (10.0^^6)

module Codewars.Exercise.Squares where

squares :: Integer -> Int -> [Integer]
squares x n = map fn [0..(n - 1)]
  where fn i = x ^ (2 ^ i)

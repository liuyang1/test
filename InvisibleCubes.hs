module Kata.InvisibleCubes where

notVisibleCubes :: Integer -> Integer
notVisibleCubes n
  | n < 2 = 0
  | otherwise = (n - 2) ^ 3

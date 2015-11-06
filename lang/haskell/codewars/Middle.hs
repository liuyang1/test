module Codewars.Kata.Middle where

-- | Return the index of the middle element.
--   The first element has index 0.
gimme :: Ord a => (a, a, a) -> Int
gimme (a, b, c)
  | (a >= b) == (a <= c) = 0
  | otherwise = 1 + gimme (b, c, a)

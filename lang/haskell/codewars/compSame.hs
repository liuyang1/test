module Codewars.Kata.Compare where

comp :: [Integer] -> [Integer] -> Bool
comp [] [] = True
comp as bs
  | length as /= length bs = False
  | otherwise = let aa = map (\x -> x * x) as
                 in all (`elem` aa) bs

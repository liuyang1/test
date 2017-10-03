module Codewars.Kata.ModSys where

comb2 [] = []
comb2 (m:ms) = map (\x -> (m,x)) ms ++ comb2 ms
iscoprime (a, b)
  | a < b = iscoprime (b, a)
  | otherwise = gcd a b == 1
check ms = all iscoprime $ comb2 ms
fromNb2Str :: Integer -> [Integer] -> String
fromNb2Str n ms
  | not (check ms) || product ms < n = "Not applicable"
  | otherwise = concatMap ((\x -> "-" ++ show x ++ "-") . (n `rem`)) ms

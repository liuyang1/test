module Collatz where

collatz :: Int -> String
collatz n
  | n == 1 = show n
  | even n = show n ++ "->" ++ collatz (n `div` 2)
  | otherwise = show n ++ "->" ++ collatz (3 * n + 1)

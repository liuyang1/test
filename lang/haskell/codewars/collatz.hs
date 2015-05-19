module Codewars.Kata.Collatz where

collatz :: Integer -> Int
collatzh 1 cnt = cnt + 1
collatzh n cnt = if rem n 2 == 0 then collatzh (n `div` 2) (cnt + 1)
                                else collatzh (3 * n + 1) (cnt + 1)
collatz n = collatzh n 0

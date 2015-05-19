module Codewars.Kata.Fib where

hlp a b n
  | a * b == n = (a, b, True)
  | a * b < n = hlp b (a + b) n
  | a * b > n  = (a, b, False)
productFib :: Integer -> (Integer, Integer, Bool)
productFib = hlp 1 1

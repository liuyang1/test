module Fibonacci where

fibonacci :: Int -> Integer
fibonacci = (map fib [0 ..] !!)
  where fib 0 = 0
        fib 1 = 1
        fib n = fibonacci (n - 2) + fib (n - 1)

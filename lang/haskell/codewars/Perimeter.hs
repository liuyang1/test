module Codewars.Kata.Perimeter where

fib 0 acc prev = acc
fib n acc prev = fib (n - 1) (acc + prev) acc

fibb n = fib n 1 0

-- sum of square of fib
-- perimeter n = fibonacci n * fibonacci (n + 1)
perimeter n = 4 * (fibb (n + 2) - 1)

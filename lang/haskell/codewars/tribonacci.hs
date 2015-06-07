module Tribonacci where

tribonacci :: Num a => (a, a, a) -> Int -> [a]
tribonacci (a, b, c) n = map trib' [0..(n - 1)]
  where trib 0 = a
        trib 1 = b
        trib 2 = c
        trib n = trib' (n - 1) + trib' (n - 2) + trib' (n - 3)
        trib' = (map trib [0..] !!)

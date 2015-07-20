module DigitalRoot where

digit10 0 xs = xs
digit10 n xs = digit10 (n `div` 10) (rem n 10: xs)

digitalRoot n
  | n < 10 = n
  | otherwise = digitalRoot . sum $ digit10 n []

-- best practice
digitalRoot' 0 = 0
digitalRoot' n = 1 + (n - 1) `mod` 9

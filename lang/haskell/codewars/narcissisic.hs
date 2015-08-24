module Narcissistic where

digit 0 xs = xs
digit n xs = digit (n `div` 10) (rem n 10: xs)

narcissistic :: Integral n => n -> Bool
narcissistic a
  | 1 == (length ds) = True
  | all (== 1) ds = False
  | otherwise = (\x -> fn ds x == a) . last $ takeWhile (\x -> fn ds x <= a) [2..]
    where ds = digit a []
          fn dd p = sum $ map (^ p) dd

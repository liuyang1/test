module Codewars.Kata.FactorialTail where

import Data.List (group)
import Control.Arrow ((&&&))

countFreq b = sum . takeWhile (/= 0) . tail . iterate (`div` b)

factorg n = map (head &&& length) . group $ hlp n 2
    where hlp n i
              | i * i > n = [n]
              | n `mod` i == 0 = i: hlp (n `quot` i) i
              | otherwise = hlp n (i + 1)

zeroes _ 1 = 0
zeroes 1 _ = error "base must greater than 1"
zeroes b n = minimum $ map (\(f, m) -> (countFreq f n) `div` (fromIntegral m)) $ factorg b

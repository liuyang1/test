module SequenceSum where

sumOfN :: Int -> [Int]
sumOfN 0 = [0]
sumOfN n = scanl1 (+) [0,signum n..n]

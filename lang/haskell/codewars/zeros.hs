module Zeros where

zeros :: Int -> Int
zeros n = sum $ map (\x -> n `div` x) $ takeWhile (<= n) $ map (5 ^) [1..]

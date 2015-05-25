module Hamming where

dive a b
  | rem a b == 0 = dive (a `div` b) b
  | otherwise = a

divl a [] = a
divl a (x:xs) = divl (dive a x) xs

hamming  :: Int -> Int
hamming idx
  | idx == 1 = 1
  | otherwise = filter (\x -> divl x [2,3,5] == 1) [2..] !! (idx - 2)

module Codewars.Kata.PascalsTriangle where
tril [] = []
tril xs@(_:t) = 1: zipWith (+) xs t ++ [1]

pascal 0 = []
pascal 1 = [[1]]
pascal n = let xs = pascal (n - 1)
               x = head xs
            in tril x: xs

pascalsTriangle :: Int -> [Int]
pascalsTriangle = concat . reverse . pascal

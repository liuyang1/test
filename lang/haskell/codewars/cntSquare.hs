module Feynman where

countSquares :: Integer -> Integer
countSquares x = sum $ map (\i -> i * i) [1..x]

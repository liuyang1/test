module Merge where

merge :: Ord a => [a] -> [a] -> [a]
merge xs [] = xs
merge [] ys = ys
merge (x:xs) (y:ys) = if x < y then x: merge xs (y:ys) else y: merge (x:xs) ys

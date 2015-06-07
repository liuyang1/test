module Xbonacci where

xbona xs = fn'
  where len = length xs
        xx = [1..len]
        fn i = if i < len then xs !! i else sum $ map (fn' . (i -)) xx
        fn' = (map fn [0..] !!)

xbonacci :: Num a => [a] -> Int -> [a]
xbonacci xs n = map (xbona xs) [0..(n-1)]

module Change where

import Data.List

chg _ 0 = 1
chg [] n = 0
chg (x:xs) n
  | x > n = chg xs n
  | otherwise = chg (x:xs) (n - x) + chg xs n

countChange n xs = chg (sortBy (flip compare) xs) n

test = countChange 100 [1, 5, 10, 25, 50]

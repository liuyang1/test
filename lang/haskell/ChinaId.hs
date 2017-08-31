module ChinaId where

import Data.Char (digitToInt)

chr2Ord 'x' = 10
chr2Ord 'X' = 10
chr2Ord x = digitToInt x

valid :: String -> Bool
valid xs = (length xs) == 18 && ((== 1) . (`mod` 11) .  sum . zipWith (*) ys . map chr2Ord $ xs)
    where ys = [7, 9, 10, 5, 8, 4, 2, 1, 6, 3, 7, 9, 10, 5, 8, 4, 2, 1]

main = print $ valid "11010519491231002X"

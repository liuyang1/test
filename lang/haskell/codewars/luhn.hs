-- https://en.wikipedia.org/wiki/Luhn_algorithm
import Data.List (cycle)
import Data.Char (digitToInt)

f sel x
  | sel == 1 = x
  | otherwise = sum . map digitToInt . show $ (sel * x)

luhn xs = (10 -) $ (`mod` 10 ) $ sum $ zipWith f (cycle [1, 2]) xs

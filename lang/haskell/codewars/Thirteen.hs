module Codewars.G964.Thirteen where

import Data.Char
import Data.List
import Data.Maybe

pat = cycle [1, 10, 9, 12, 3, 4]

-- This is smart end condition <= 100
thirt :: Integer -> Integer
-- thirt = head . dropWhile (>= 100) . iterate go
thirt = fromJust . find (< 100) . iterate go
    where go n = sum $ zipWith (*) pat (reverse $ map (toInteger . digitToInt) $ show n)

f :: Integer -> Integer
f = sum . zipWith (*) pat . reverse . map (read . pure) . show

-- read . pure == toInteger . digitToInt
-- head . dorpWhile f = fromJust . find f 

-- find first repeat one
thirt1 = fst . fromJust . find (uncurry (==)) . (zip <*> tail) . iterate f

module  Codewars.G964.WeightSort where

import Data.List (sortBy, sortOn)
import Data.Function (on)
import Data.Char (ord, digitToInt)
import Control.Arrow ((&&&))

-- orderWeight :: [Char] -> [Char]
orderWeight = unwords . sortBy cmp . words

cmp = (compare `on` digitSum) `mappend` compare

digitSum = sum . map ((\x -> x - ord '0') . ord)

orderWeight1 = unwords . sortOn (sum . map digitToInt &&& id) . words

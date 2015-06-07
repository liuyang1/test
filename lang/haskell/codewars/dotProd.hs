module MinimumDot where
import Data.List

minDot :: (Ord a, Num a) => [a] -> [a] -> a
minDot xs ys = sum $ zipWith (*) (sort xs) (sortBy (flip compare) ys)

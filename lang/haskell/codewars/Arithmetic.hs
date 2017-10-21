module Codewars.Kata.Arithmetic where

import Data.Maybe (fromJust, isJust, isNothing)
import Data.Function (on)

-- must consider negative difference sequence
miss3 :: Integral a => a -> a -> a -> Maybe a
miss3 x y z = case (compare `on` abs) (y - x) (z - y) of
                    EQ -> Nothing
                    GT -> Just $ (x + y) `div` 2
                    LT -> Just $ (y + z) `div` 2

findMissing :: Integral n => [n] -> n
findMissing xs = fromJust $ head $ dropWhile isNothing ys
    where ys = zipWith3 miss3 xs (tail xs) (tail (tail xs))

-- better solution
findMissing' :: Integral a => [a] -> a
findMissing' lst@(x:y:z:_)
  | y - x == z - y = findMissing' $ tail lst
  | otherwise = x + z - y

-- test code
test f = do
    print $ f [1, 2, 3, 4, 6]
    print $ f [1, 3, 5, 9]
    print $ f [1, 2, 4, 5]
    print $ f [1, 2, 4]
    print $ f [1, 3, 4]
    print $ -1 == f [1, 0, (-2)]
    print $ 0 == f [1, (-1), (-2)]

main = do
    test findMissing
    test findMissing'

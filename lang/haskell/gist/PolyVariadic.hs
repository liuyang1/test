module PolyVariadic where

class SumRes r where
    sumOf :: Integer -> r

instance SumRes Integer where
    sumOf = id

-- | sumOf could add variadic arguments
-- >>> sumOf 1 2 3 :: Integer
-- 6
-- >>> sumOf 4 5 :: Integer
-- 9
instance (Integral a, SumRes r) => SumRes (a -> r) where
    sumOf x = sumOf . (x +) . toInteger

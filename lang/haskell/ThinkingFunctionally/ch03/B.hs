import Data.Function (on)

a :: Integer
a = 2 ^ 3

b :: Double
b = 2 ^^ 3

c :: Double
c = 2 ** 3

vv :: (Fractional a, Integral b) => a -> b -> a
vv _ 0 = 1
vv x 1 = x
vv x n
    | n < 0 = 1 / vv x (-n)
    | even n = y * y
    | otherwise = y * y * x
    where y = vv x (n `div` 2)

test h = h <$> [(-5)..5] <*> [(-5)..5]

verify f g = and $ (zipWith (==) `on` test) f g

main = print $ verify (^^) (vv)

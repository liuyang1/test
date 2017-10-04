import Data.Function (on)

-- a = -2 + 3
-- b = 3 + -2
c = 3 + (-2)
d = subtract 2 3
-- e = 2 + subtract 3

subtract0 = flip (-)

-- 验证两个二元函数是否相等
verify2 :: (Integer -> Integer -> Integer) -> (Integer -> Integer -> Integer) -> Bool
verify2 f g = and $ (zipWith (==) `on` test) f g
    where test h = h <$> [1..5] <*> [1..5]

main = do
    mapM (print . (== 1)) [c, d]
    print $ verify2 subtract subtract0


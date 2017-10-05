import Data.List (group, sort, nub)

nub0, nub1, nub2 :: (Ord a) => [a] -> [a]
nub0 = map head . group . sort

-- O(n^2), 但是稳定，保持位置不变
nub1 [] = []
nub1 (x:xs) = x: nub1 (filter (/= x) xs)

nub2 = rmdups . sort

rmdups [] = []
rmdups (x:xs) = x: rmdups (dropWhile (== x) xs)

test f xss = all (\xs -> (sort . nub) xs == (sort . f) xs) xss

xss = [[0..10], [0..10] ++ [5], [], [1], [1, 2]]

main = do
        print $ test nub0 xss
        print $ test nub1 xss
        print $ test nub2 xss

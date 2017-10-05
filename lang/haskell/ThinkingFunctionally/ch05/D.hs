import Data.List (sort)

nodups :: (Ord a) => [a] -> Bool
nodups xs = and $ zipWith (/=) ys (tail ys)
    where ys = sort xs

main = do
        print $ nodups [1..10]
        print $ False == nodups ([1..10] ++ [5])

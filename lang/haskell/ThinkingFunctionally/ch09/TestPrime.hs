import Data.List ((\\))

primes = 2: ([3..] \\ composites)

composites = mergeAll multiples

multiples = [map (n *) [n..] | n <- [2..]]

merge :: Ord a => [a] -> [a] -> [a]
merge xx@(x:xs) yy@(y:ys)
    | x < y = x: merge xs yy
    | x == y = x: merge xs ys
    | otherwise = y: merge xx ys

xmerge (x:xs) ys = x: merge xs ys

-- mergeAll = foldr1 xmerge
mergeAll (xs:xss) = xmerge xs (mergeAll xss)

-- XXX: 还是不行啊
main = do
        print $ take 10 $ primes

-- : is non-strict for second argument
-- so enumFrom supported infinite list
enumFrom0 m = m: enumFrom0 (m + 1)

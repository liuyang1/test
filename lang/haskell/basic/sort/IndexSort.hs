module IndexSort where

-- WARNING: index sort limit range of number
-- for exmaple: we limit to [0, 255]

import Data.List (partition)
import Data.Bits ((.&.))

pow2 0 = 1
pow2 n 
    | n `mod` 2 == 0 = p * p
    | otherwise = p * p * 2
        where p = pow2 (n `quot` 2)

index _ [] = []
index (-1) xs = xs
index 8 xs = xs
index n xs = index (n + 1) (l ++ r)
    where (l, r) = partition (\x -> x .&. (pow2 n) == 0) xs

indexSort xs = index 0 xs

sort = indexSort

main = do
        print "test on simple test"
        print $ [2, 3, 4, 10] == sort [10, 2, 4, 3]
        print "test on duplicate element"
        print $ [2, 2, 3, 4, 10, 10] == sort [10, 2, 2, 4, 3, 10]
        print "test on reverse sequence"
        print $ [1..10] == sort [10, 9..1]
        print "test on corner case"
        print $ [1] == sort [1]
        print $ ([]::[Integer]) == sort ([]::[Integer])

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
index 8 xs = xs
index n xs = index (n + 1) (l ++ r)
    where m = pow2 n
          (l, r) = partition (\x -> x .&. m == 0) xs

indexSort = index 0

-- 20180914 new implementation
-- - don't based on Bits Operation (so we may extend to other base, not only 2)
-- - support unlimited number instead of maximum threshould

radicalB b 0 = []
radicalB b x = r: radicalB b q
    where (q, r) = x `divMod` b

deradicalB b xs = sum $ zipWith (\c e -> c * b ^ e) xs [0..]

elemAt xs n = if length xs > n then xs !! n else 0

-- quick binary bucket sort based on IDX element
resort2 idx = uncurry (++) . partition (\x -> 0 == elemAt x idx)

resortB b idx = foldl (++) [] . go 0
    where go i xs = if i == b then [] else pos: go (i + 1) neg
              where (pos, neg) = partition (\x -> i == elemAt x idx) xs

radixSortB b = map (deradicalB b) . conv . map (radicalB b)
    where conv xs = go 0 xs
              where lm = if null xs then 0 else maximum $ map length xs
                    go i xs
                      | i == lm = xs
                      | otherwise = go (i + 1) (resortB b i xs)

radixSort = radixSortB 10

insert y [] = [y]
insert y xx@(x:xs)
  | y < x = y:xx
  | otherwise = x: insert y xs

insertSort [] = []
insertSort (x: xs) = insert x (insertSort xs)

seq0 = [0, 8, 4, 12, 2, 10, 6, 14, 1, 9, 5, 13, 3, 11, 7, 15]

test f = do
        print "test on simple test"
        print $ [2, 3, 4, 10] == f [10, 2, 4, 3]
        print $ [0..15] == f seq0
        print "test on duplicate element"
        print $ [2, 2, 3, 4, 10, 10] == f [10, 2, 2, 4, 3, 10]
        print "test on reverse sequence"
        print $ [1..10] == f [10, 9..1]
        print "test on corner case"
        print $ [0] == f [0]
        print $ [1] == f [1]
        print $ ([]::[Integer]) == f ([]::[Integer])

main = do
    print "---- indexSort"
    test indexSort
    print "---- radixSort"
    test radixSort
    print "---- insertSort"
    test insertSort

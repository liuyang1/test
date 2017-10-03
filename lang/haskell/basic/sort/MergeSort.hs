module MergeSort where

import Data.Function (on)

merge [] ys = ys
merge xs [] = xs
merge xx@(x:xs) yy@(y:ys) = case compare x y of
                          LT -> x: merge xs yy
                          EQ -> x: merge xs yy
                          GT -> y: merge xx ys

mergeSort [] = []
mergeSort [x] = [x]
mergeSort xs = (merge `on` mergeSort) l r
    where half = length xs `quot` 2
          (l, r) = splitAt half xs


main = do
        print "test on simple test"
        print $ [2, 3, 4, 10] == mergeSort [10, 2, 4, 3]
        print "test on duplicate element"
        print $ [2, 2, 3, 4, 10, 10] == mergeSort [10, 2, 2, 4, 3, 10]
        print "test on reverse sequence"
        print $ [1..10] == mergeSort [10, 9..1]
        print "test on corner case"
        print $ [1] == mergeSort [1]
        print $ ([]::[Integer]) == mergeSort ([]::[Integer])

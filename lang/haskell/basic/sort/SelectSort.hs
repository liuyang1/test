module SelectSort where

import Data.List ((\\))

-- remove one time of a in xs
-- It's same with (\\) in Data.List
remove [] _ = []
remove (x:xs) a
    | x == a = xs
    | otherwise = x: remove xs a

selectSort [] = []
selectSort xs = m : selectSort (xs \\ [m])
    where m = minimum xs

sort = selectSort

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

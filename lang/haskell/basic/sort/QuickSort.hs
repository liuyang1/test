module QuickSort where
import Data.List (partition)

qsort :: Ord a => [a] -> [a]
qsort [] = []
qsort (x:xs) = qsort lhs ++ [x] ++ qsort rhs
               where (lhs, rhs) = partition (< x) xs

main = print $ qsort [1, 4, 2, 7, 4]

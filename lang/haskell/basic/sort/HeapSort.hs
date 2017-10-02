module HeapSort where

-- basic indexing style
root = 0
parent n = (n - 1) `quot` 2
leftChild n = 2 * n + 1
rightChild n = 2 * n + 2

-- How to build heap
insert [] x = [x]
insert xs x = swim (xs ++ [x])

heapBuild xs = foldl insert [] xs

-- sorry for permitive style
enum xs = zip xs [0..(length xs)]

setv xs pos a = map (\(x, i) -> if i == pos then a else x) $ enum xs

exch xs p0 p1 = setv (setv xs p0 v1) p1 v0
    where v0 = xs !! p0
          v1 = xs !! p1

-- basic operation, SWIM and SINK
swimOne xs n = let p = parent n
                in if xs !! p > xs !! n then swimOne (exch xs p n) p else xs

swim xs = swimOne xs ((length xs) - 1)

sinkOne xs n
    | n >= len = xs
    | nv == m = xs
    | lv == m = sinkOne (exch xs l n) l
    | rv == m = sinkOne (exch xs r n) r
    where len = length xs
          l = leftChild n
          r = rightChild n
          nv = xs !! n
          lv = if l >= len then nv else xs !! l
          rv = if r >= len then nv else xs !! r
          m = minimum [lv, rv, nv]

sink xs = sinkOne xs root

-- to check isHeap or not
isHeapOne xs i l r = (l >= len || x < lv) && (r >= len || x < rv)
    where len = length xs
          x = xs !! i
          lv = xs !! l
          rv = xs !! r

isHeapL xs i
    | i >= length xs = True
    | otherwise = isHeapL xs lft && isHeapL xs rgt && isHeapOne xs i lft rgt
    where lft = leftChild i
          rgt = rightChild i

isHeap xs = isHeapL xs root

-- How to delete element from sink
heapDel [] = (0, [])
heapDel (x:xs) = (x, sink xs)

heapSeq [] = []
heapSeq xs = v: heapSeq ys
        where (v, ys) = heapDel xs

heapSort = heapSeq . heapBuild

test sort = do
        print "test on simple test"
        print $ [2, 3, 4, 10] == sort [10, 2, 4, 3]
        print "test on duplicate element"
        print $ [2, 2, 3, 4, 10, 10] == sort [10, 2, 2, 4, 3, 10]
        print "test on reverse sequence"
        print $ [1..10] == sort [10, 9..1]
        print "test on corner case"
        print $ [1] == sort [1]
        print $ ([]::[Integer]) == sort ([]::[Integer])

main = do
        test heapSort

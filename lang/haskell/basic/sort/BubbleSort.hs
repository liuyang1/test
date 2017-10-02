module BubbleSort where

-- naiveBubble always reorder sequence from head to tail
-- and naiveBubbleSort repeat (LENGTH xs) times.
naiveBubble [] = []
naiveBubble [x] = [x]
naiveBubble (x:y:xs) = a:(naiveBubble (b:xs))
    where (a, b) = if x <= y then (x, y) else (y, x)

naiveBubbleSort xs = (iterate naiveBubble xs) !! (length xs)

-- XXX bubble reorder sequence until find the order is correct
bubble [] = ([], False)
bubble [x] = ([x], False)
bubble xx@(x:y:xs)
    | x <= y = (xx, False) -- wrong, need continue to reorder
    | otherwise = (y:(fst $ bubble (x:xs)), True)

-- bubbleSort reoder sequence until find not change after reorder last time
bubbleSort [] = []
bubbleSort xs = bubbleSort (init ys) ++ [last ys]
    where ys = naiveBubble xs

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
        test naiveBubbleSort
        test bubbleSort

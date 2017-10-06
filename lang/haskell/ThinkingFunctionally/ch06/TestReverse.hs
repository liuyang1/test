-- 复杂度为O(n)
reverse0 = foldl (flip (:)) []

-- 以下两种方法效率低下的原因在于列表的append操作需要O(n)
-- 因此整体的复杂度为O(n^2)
reverse1 [] = []
reverse1 (x:xs) = reverse xs ++ [x]

reverse2 = foldr snoc []
    where snoc x xs = xs ++ [x]

main = do
        print $ reverse0 "Hello, World!"
        print $ reverse1 "Hello, World!"
        print $ reverse2 "Hello, World!"

-- TODO: 使用测量时间的函数，来profile下
testCplx = do
        print $ length $ reverse0 [0..10000000]
        print $ length $ reverse1 [0..10000000]

a = [[], []] -- 2 x 0
b = [] -- 0 x n

transpose, transpose' :: [[a]] -> [[a]]
transpose [] = repeat [] -- 因为转置为nx0,n的长度是不确定的
-- 第一行的正确性，是在没有第二行定义的情况下体现出来的。
-- transpose [xs] = [[x] | x <- xs] -- 1xn的矩阵，转置为nx1矩阵
transpose (xs:xss) = zipWith (:) xs (transpose xss)

transpose' ([]:_) = []
transpose' xss = map head xss: transpose' (map tail xss)

m0 = [[1, 1], [0, 1]]

test tr = do
            print $ tr m0
            print $ tr [[], []]
            print $ tr [[1, 0]]
            print $ take 10 $ tr [] -- 无穷列表

main = do
        test transpose
        test transpose'

import Data.List (transpose)
-- 定义矩阵上的运算

type Mat = [[Int]]

addNum :: Int -> Mat -> Mat
addNum n = map (map (+ n))

sumMat :: Mat -> Int
sumMat = sum . (map sum)

addMat :: Mat -> Mat -> Mat
addMat = zipWith (zipWith (+))

mulVec :: [Int] -> [Int] -> Int
mulVec v0 v1 = sum $ zipWith (*) v0 v1

mulMat :: Mat -> Mat -> Mat
mulMat m0 m1 = [[i `mulVec` j| j <- m1']| i <- m0]
    where m1' = transpose m1
-- 注意，选取第i行与第j列进行乘法运算

m0 = [[1, 1], [0, 1]]

main = do
        print $ addNum 3 m0
        print $ sumMat m0
        print $ addMat m0 m0
        print $ mulMat m0 m0

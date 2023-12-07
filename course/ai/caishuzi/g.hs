import Debug.Trace
import Data.List
--import Data.List (tails,subsequences,permutations, unfoldr, genericLength, sort, group, nubBy)
import Data.Maybe

-- 如何从等价集合中寻找合理的组合
-- 比如第一次选择0123，那么就划分为两个等价集合[[0,1,2,3],[4,5,6,7,8,9]] 4
-- 从第一个集合中选择k个，然后从后续集合列表中选择（n-k）个，然后两者混合排列（按照类别区分位置）
-- [[0,1,2,3],
--  [0,1,2,4],[0,1,4,2],[0,4,1,2],[4,0,1,2],
--  [0,1,4,5],[0,4,1,5],[0,4,5,1],[4,0,1,5],[4,0,5,1],[4,5,0,1],
--  [0,4,5,6],[4,0,5,6],[4,5,0,6],[4,5,6,0],
--  [4,5,6,7]]
--  1+4+6+4+1=16
--  P(4)/P(2)/P(2)=6

-- 第一个函数，只是选择，没有按组排列的功能
build :: [[a]] -> Int -> [[a]]
build [] n = []
build ([]:xxs) n = build xxs n
build (xs:xxs) 1 = [head xs]: build xxs 1
build (xs:xxs) n = map (\nx-> (head xs): nx) (build (tail xs: xxs) (n-1)) ++
                   build xxs n

build1 :: [[a]] -> Int -> [[a]]
build1 [] n = []
build1 ([]:xs) n = build1 xs n
build1 xs 1 = xs
-- build1 (xs:xxs) n = _

-- merge2 [0,1,2,3] [] = [[0,1,2,3]]
-- merge2 [0,1] [4,5] = [[0,1,4,5],[0,4,1,5],[0,4,5,1],[4,0,1,5],[4,0,5,1],[4,5,0,1]]
merge2 :: [a] -> [a] -> [[a]]
merge2 xs [] = [xs]
merge2 [] ys = [ys]
merge2 xs@(x:xr) ys@(y:yr) = map (x:) (merge2 xr ys) ++ map (y:) (merge2 xs yr)

--mergen :: forall a. [[a]] -> [[a]]
mergen [] = []
mergen ([]:xxs) = mergen xxs
mergen (xs:xxs) = trace ((show xs) ++ "," ++ (show xxs)) $ map ((head xs):) (mergen ((tail xs):xxs))


main = do
    print $ build [[1,2]] 2
    print $ build [[1,2],[3]] 2
    print $ build [[1,2,3]] 2
    print $ build [[1,2],[3,4]] 2
    print $ build [[0,1,2,3],[4,5,6,7,8,9]] 4

module Sudoku where

import Data.List (splitAt, transpose, lines)
import Data.Char (digitToInt)
import System.IO

proc s = map (map (map digitToInt) . tail) $ splitSeg (matSize + 1) $ lines s

loadMats = do
    s <- readFile "p096_sudoku.txt"
    mapM_ print $ id $ (!! 1) $ proc s
    mapM_ print $ sudoku $ (!! 1) $ proc s

type Mat = [[Int]]

puzzle0 = [[5,3,0,0,7,0,0,0,0]
          ,[6,0,0,1,9,5,0,0,0]
          ,[0,9,8,0,0,0,0,6,0]
          ,[8,0,0,0,6,0,0,0,3]
          ,[4,0,0,8,0,3,0,0,1]
          ,[7,0,0,0,2,0,0,0,6]
          ,[0,6,0,0,0,0,2,8,0]
          ,[0,0,0,4,1,9,0,0,5]
          ,[0,0,0,0,8,0,0,7,9]] :: Mat
puzzle1 = [[0,0,4,0,0,5,7,0,0]
          ,[0,0,0,0,0,9,4,0,0]
          ,[3,6,0,0,0,0,0,0,8]
          ,[7,2,0,0,6,0,0,0,0]
          ,[0,0,0,4,0,2,0,0,0]
          ,[0,0,0,0,8,0,0,9,3]
          ,[4,0,0,0,0,0,0,5,6]
          ,[0,0,5,3,0,0,0,0,0]
          ,[0,0,6,1,0,0,9,0,0]] :: Mat
cands = [1..9]
boxSize = 3
matSize = 9
-- 所有的位置坐标
poss = sequence [[0..(matSize - 1)], [0..(matSize - 1)]]

-- 返回棋盘中对应的行或者列,或者所处的3x3的小方格
row, col :: Mat -> Int -> [Int]
row m x = m !! x
col m y = transpose m !! y

regionN n x = take n [(roundN n x)..]
    where roundN n x = (x `div` n) * n

box :: Mat -> Int -> Int -> [Int]
box m x y = map (\(i:j:_) -> (m !! i) !! j) $ boxPos x y
    where boxPos x y = sequence [(regionN boxSize x), (regionN boxSize y)]
-- 找到所有的备选项
fcand :: Mat -> Int -> Int -> [Int]
fcand m x y = filter (`notElem` c) cands
    where c = row m x ++ col m y ++ box m x y

updateCell :: Mat -> Int -> Int -> Int
updateCell m x y
    | v /= 0 = v
    | v == 0 && length vs == 1 = head vs -- 只有在一个可选择结果的时候，直接选择
    | otherwise = 0
    where v = m !! x !! y
          vs = fcand m x y

-- 按段划分，类似于Python中的xs[::9]
--splitSeg :: Int -> [Int] -> Mat
splitSeg n [] = []
splitSeg n xs = h: splitSeg n t
    where (h, t) = splitAt n xs

rebuildMat :: [Int] -> Mat
rebuildMat = splitSeg matSize

-- 策略非常简单
-- 采用直接推断的方式: 同行同列同小格的元素不再出现,如果剩余备选项只有一个，则固定选择。
-- 否则，进入到下一个推测中
sudoku :: Mat -> Mat
sudoku m = if nm == m then m else sudoku nm
    where nm = updateCand m
          updateCand m = rebuildMat . map (\(i:j:_) -> updateCell m i j) $ poss

showMat m = mapM_ print m

test puzzle = do
        print "orignal puzzle"
        showMat puzzle
        print "solution"
        showMat $ sudoku puzzle

main = do
        test puzzle0
        test puzzle1

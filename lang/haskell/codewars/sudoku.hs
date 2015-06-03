module Sudoku where

import Data.List
m0 = 3
ms = [0..(m0 - 1)]
mm x = map (\i -> i + 3 * x) ms
mmm x y = sequence [mm x,mm y]
gm m x y = map (\(i:j:_) -> (m !! i) !! j) $ mmm x y

{-
puzzle = [[5,3,0,0,7,0,0,0,0],
          [6,0,0,1,9,5,0,0,0],
          [0,9,8,0,0,0,0,6,0],
          [8,0,0,0,6,0,0,0,3],
          [4,0,0,8,0,3,0,0,1],
          [7,0,0,0,2,0,0,0,6],
          [0,6,0,0,0,0,2,8,0],
          [0,0,0,4,1,9,0,0,5],
          [0,0,0,0,8,0,0,7,9]]
-}
cands = [1..9]
row m x = m !! x
col m y = transpose m !! y
sqr m x y = gm m (div x 3) (div y 3)
fcons m x y = let c = filter (/= 0) (row m x ++ col m y ++ sqr m x y)
               in filter (`notElem` c) cands
ff m x y = let v = m !! x !! y
            in if v == 0
                  then let vv = fcons m x y in if length vv == 1 then head vv else 0
                  else v

splith n [] i r0 r1 = if r0 /= [] then reverse r0: r1 else r1
splith n xs@(x:t) i r0 r1 = if i == n then splith n xs 0 [] (reverse r0:r1) else splith n t (i + 1) (x:r0) r1
splitEvery n xs = reverse $ splith n xs 0 [] []
sudoku :: [[Int]] -> [[Int]]
sudoku m = let nm = splitEvery 9 . map (\(i:j:_) -> ff m i j) $ sequence [[0..8], [0..8]]
            in if nm == m then m else sudoku nm

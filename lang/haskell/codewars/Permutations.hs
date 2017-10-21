module Codewars.Kata.Permutations (permutations) where

import Data.List (nub, splitAt)

idxInsert xs x i = ys ++ [x] ++ zs
    where (ys, zs) = splitAt i xs
insertAll xs x = nub $ map (idxInsert xs x) [0..(length xs)]

permutations :: String -> [String]
permutations [] = [[]]
permutations (x:xs) = nub $ concatMap (`insertAll` x) (permutations xs)

main = do
    print $ permutations "a"
    print $ permutations "ab"
    print $ permutations "aabb"

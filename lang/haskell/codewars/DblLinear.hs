-- module Codewars.G964.DblLinear where
module Main where

import Data.Maybe
import Data.List (nub)

-- u[0] = 1
-- if x in U, then 2 * x + 1, 3 * x + 1 in U, too.
-- no other elements in U

-- merge, remove duplicate
merge xx@(x:xs) yy@(y:ys)
    | x < y = x: merge xs yy
    | x == y = x: merge xs ys
    | otherwise = y: merge xx ys
merge xs [] = xs
merge [] ys = ys

-- keep entire xs while loop
f n xs i
  | i == n = v
  | otherwise = f n (merge xs [2 * v + 1, 3 * v + 1]) (i + 1)
  where v = xs !! i

-- drop header of xs while loop
f1 n (x:xs) i
  | i == n = x
  | otherwise = f1 n (merge xs [2 * x + 1, 3 * x + 1]) (i + 1)

-- based on tree solution
data Tree a = Empty
            | Node a Int (Tree a) (Tree a)
            deriving (Show)

leftChild Empty = Nothing
leftChild (Node _ _ left _) = Just left

rightChild Empty = Nothing
rightChild (Node _ _ _ right) = Just right

height Empty = 0
height (Node _ h _ _) = h

updateHeight Empty = Empty
updateHeight (Node a _ left right)= Node  a (max (height left) (height right) + 1) left right

rotateLeft Empty = Empty
rotateLeft (Node a h alpha (Node b _ beta gamma)) = Node b h (updateHeight $ Node a 1 alpha beta) gamma
-- ratateLeft (Node a h alpha Empty) =
rorateRight (Node a h (Node b _ alpha beta) gamma) = Node b h alpha (updateHeight $ Node b 1 beta gamma)

bal Empty = Empty
bal n@(Node a _ left right)
  | hl - 3 > hr = rotateLeft n
  | hl < hr - 3 = rorateRight n
  | otherwise = n
  where hl = height left
        hr = height right

treeInsert x Empty = Node x 1 Empty Empty
treeInsert x n@(Node a _ left right)
  | x == a = n
  | x < a = updateHeight $ Node a 0 (treeInsert x left) right
  | otherwise = updateHeight $ Node a 0 left (treeInsert x right)

treePeekMin Empty = Nothing
treePeekMin (Node a _ Empty _) = Just a
treePeekMin (Node _ _ left _) = treePeekMin left

treeDelMin Empty = Empty
treeDelMin (Node a _ Empty right) = right
treeDelMin (Node a _ left right) = updateHeight $ Node a 0 (treeDelMin left) right

listToTree = foldr treeInsert Empty

g n t i
  | i == n = nt
  | otherwise = g n (treeInsert (3 * v + 1) (treeInsert (2 * v + 1) nt)) (i + 1)
  -- | otherwise = g n (treeInsert (2 * v + 1) (treeInsert (3 * v + 1) nt)) (i + 1)
  where v = fromJust $ treePeekMin t
        nt = treeDelMin t

test n = g n (treeInsert 1 Empty) 0

testf n =  f n [1] 0
testf1 n = f1 n [1] 0

-- test and interface
dblLinear :: Int -> Integer
-- dblLinear = undefined
dblLinear n = f1 n [1] 0
-- dblLinear n = g n (treeInsert 1 Empty) 0

-- main = do print $ dblLinear 2000

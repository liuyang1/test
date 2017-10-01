module BinTree where

import Data.Maybe

-- *** demo for how to define Algebraic Data Type
data Tree a = EmptyTree
            | Node a (Tree a ) (Tree a)
            deriving (Show, Read, Eq)
            -- *** demo for how to use deriving

singleton :: a -> Tree a
singleton x = Node x EmptyTree EmptyTree

-- insert x if x not in tree
treeInsert :: (Ord a) => a -> Tree a -> Tree a
treeInsert x EmptyTree = singleton x
treeInsert x (Node a left right) = case (compare x a) of
                                       LT -> Node a (treeInsert x left) right
                                       EQ -> Node a left right
                                       GT -> Node a left (treeInsert x right)

-- check x is in tree or not
treeIn x EmptyTree = False
treeIn x (Node a left right) = case (compare x a) of
                                   LT -> treeIn x left
                                   EQ -> True
                                   GT -> treeIn x right

-- check tree is matched constraint
-- left <= a <= right
isTree EmptyTree = True
isTree (Node a left right)
    | lv /= Nothing && (fromJust lv) > a = False
    | rv /= Nothing && (fromJust rv) < a = False
    | otherwise = isTree left && isTree right
    where lv = treeVal left
          rv = treeVal right
          treeVal EmptyTree = Nothing
          treeVal (Node a _ _) = Just a

listToTree = foldr treeInsert EmptyTree

-- test code
nums = [8, 6, 7, 4]
tree = listToTree nums
main = do
        print tree
        print $ True == isTree tree
        print $ False == isTree (Node 3 (Node 4 EmptyTree EmptyTree) EmptyTree)
        print $ True == treeIn 7 tree
        print $ False == treeIn 5 tree

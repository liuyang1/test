import Data.List

member :: Eq a => a -> [a] -> Bool
member x [] = False
member x (y:ys) | x == y = True
  | otherwise = member x ys

mystery = 1 : map (*2) mystery

ints = scanl1 (+) . zipWith (*) [0..] . concat $ repeat [-1, 1]

merge = concat . transpose
merge2 l r = merge [l, r]
ints1 = 0: merge2 [1..] [(-1), (-2)..]

data Tree a = EmptyTree
  | Node a (Tree a) (Tree a)
  deriving (Show, Read)

inorder :: Tree a -> [a]
inorder EmptyTree = []
inorder (Node x l r) = inorder l ++ [x] ++ inorder r

postoder EmptyTree = []
postoder (Node x l r) = postoder l ++ postoder r ++ [x]

preorder EmptyTree = []
preorder (Node x l r) = [x] ++ preorder l ++ preorder r

small = (Node 4 EmptyTree (Node 5 EmptyTree EmptyTree))
big = (Node 10 small (Node 20 EmptyTree small))

data List a = EmptyList
  | LNode a (List a)
  deriving (Read, Show)

append EmptyList ys = ys
append (LNode a b) ys = LNode a (append b ys)

lst = LNode 2 (LNode 1 EmptyList)

mymap _ EmptyList = EmptyList
mymap f (LNode a b) = LNode (f a) (mymap f b)

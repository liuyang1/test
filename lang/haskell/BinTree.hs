data Tree a = EmptyTree
            | Node a (Tree a ) (Tree a)
            deriving (Show, Read, Eq)
singleton :: a -> Tree a
singleton x = Node x EmptyTree EmptyTree

treeInsert :: (Ord a) => a -> Tree a -> Tree a
treeInsert x EmptyTree = singleton x
treeInsert x (Node a left right)
    | x == a = Node x left right
    | x < a = Node a (treeInsert x left) right
    | x > a = Node a left (treeInsert x right)

nums = [8, 6, 7, 4]
tree = foldr treeInsert EmptyTree nums
main = print tree

--------------------------------------------------------------------------------
-- Homework 04

func1 [] = 1
func1 (x:xs)
  | even x = (x - 2) * func1 xs
  | otherwise = func1 xs

func1_ = product . map (\x -> x - 2) . filter even

func2 0 = 0
func2 1 = 0
func2 n | even n = n + func2 (n `div` 2)
  | otherwise = func2 (3 * n + 1)

func2_ = sum . filter even . takeWhile (> 1) . iterate fn
    where fn n | even n = n `div` 2
               | otherwise = 3 * n + 1

--------------------------------------------------------------------------------
-- Folding with trees
data Tree a = Leaf
  | Node Integer (Tree a) a (Tree a)
  deriving (Show, Eq)

height Leaf = 0
height (Node h _ _ _) = h

insert x Leaf = Node 1 Leaf x Leaf
insert x (Node h left root right) = Node h_ l_ root r_
    where (l_, r_) = if height left < height right then (insert x left, right)
                                                   else (left, insert x right)
          h_ = (max (height l_) (height r_)) + 1

foldTree = foldr insert Leaf

xor = odd . length . filter (== True)

map' f = foldr fn []
    where fn x acc = f x: acc

myFoldl f base xs = foldr (flip f) base (reverse xs)

cartProd xs ys = [(x, y) | x <- xs, y <- ys]

sieveSundaram n = fn [3,5..(2 * n+2)]
    where fn [] = []
          fn (x:xs) = x: (fn (filter (\i -> i `mod` x /= 0) xs))

data Tree a = Empty | Branch a (Tree a) (Tree a)
              deriving (Show, Eq)

leaf x = Branch x Empty Empty

tree1 = Branch 'a' (Branch 'b' (leaf 'd')
                               (leaf 'e'))
                   (Branch 'c' Empty
                               (Branch 'f' (leaf 'g')
                                           Empty))

-- problem 55
cbal 0 = [Empty]
cbal 1 = [leaf 'x']
cbal n
  | n `mod` 2 == 1 = treeX <$> leafs <*> leafs
  | otherwise = (treeX <$> leafs <*> rights) ++ (treeX <$> rights <*> leafs)
  where r = (n - 1) `div` 2
        leafs = cbal r
        rights = cbal (n - 1 - r)
        treeX = Branch 'x'

cbalNum 0 = 1
cbalNum 1 = 1
cbalNum n
  | n `mod` 2 == 1 = n2 * n2
  | otherwise = 2 * n1 * n2
  where r = (n - 1) `div` 2
        n2 = cbalNum r
        n1 = cbalNum (n - 1 - r)

-- problem 56
mirror Empty Empty = True
mirror (Branch _ l0 r0) (Branch _ l1 r1) = mirror l0 r1 && mirror r0 l1
mirror _ _ = False

symmetric Empty = True
symmetric (Branch _ l r) = mirror l r

-- problem 57
insertNode0 Empty x = Branch x Empty Empty
insertNode0 (Branch y l r) x
  | x < y = Branch y (insertNode0 l x) r
  | otherwise = Branch y l (insertNode0 r x)

insertNode x Empty = Branch x Empty Empty
insertNode x (Branch y l r)
  | x < y = Branch y (insertNode x l) r
  | otherwise = Branch y l (insertNode x r)

insertNode2 :: Ord a => Tree a -> a -> Tree a
insertNode2 = flip insertNode

construct :: Ord a => [a] -> Tree a
construct = foldl insertNode0 Empty

-- problem 58
symCbalTrees = filter symmetric . cbal

-- problem 59
hbalTree 0 = [Empty]
hbalTree 1 = [Branch 'x' Empty Empty]
hbalTree n = treeX h2 h1++ treeX h1 h2 ++ treeX h1 h1
    where h1 = hbalTree (n - 1)
          h2 = hbalTree (n - 2)
          treeX xs ys = Branch 'x' <$> xs <*> ys

-- problem 60
minNodes 0 = 0
minNodes 1 = 1
minNodes n = 1 + minNodes (n - 1) + minNodes (n - 2)

-- n nodes construct to tree with max height
maxHeight n = length $ takeWhile (<= n) $ map minNodes [1..]

hbalTreeNodes 0 = [Empty]
hbalTreeNodes 1 = [Branch 'x' Empty Empty]
hbalTreeNodes n = 

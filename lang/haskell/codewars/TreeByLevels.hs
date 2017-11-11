module TreeByLevels where

-- import TreeByLevels.TreeNode

-- struct predefined
data TreeNode a = TreeNode {
  left  :: Maybe (TreeNode a),
  right :: Maybe (TreeNode a),
  value :: a
  } deriving Show

nextLevel = concatMap f
    where f Nothing = []
          f (Just (TreeNode left right value)) = [left, right]

values = concatMap f
    where f Nothing = []
          f (Just (TreeNode _ _ value)) = [value]

treeByLevels :: Maybe (TreeNode a) -> [a]
treeByLevels n = values $ concat $ takeWhile (not . null) $ iterate nextLevel [n]

-- test code
buildTree :: [a] -> Maybe (TreeNode a)
buildTree l = fst $ walk $ split 1 l
  where split _ [] = []
        split n l = h : split (2*n) t
          where (h, t) = splitAt n l
        walk [] = (Nothing, [])
        walk ls@([] : _) = (Nothing, ls)
        walk ((h : t) : ls) = (Just $ TreeNode l r h, t : ls'')
          where (l, ls') = walk ls
                (r, ls'') = walk ls'

testBuildTree x = treeByLevels (buildTree x) -- == x

main = testBuildTree [2, 8, 9, 1, 3, 4, 5]

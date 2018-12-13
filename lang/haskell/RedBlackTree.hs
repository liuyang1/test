module RedBlackTree where


data Color = Red | Blk deriving (Show, Read, Eq)
data RBTree a = Nil
              | RBTree { color :: Color
              , val :: a
              , num :: Int
              , left :: (RBTree a)
              , right :: (RBTree a)
                       } deriving (Show, Read, Eq)

initNode x = RBTree {color = Red, val = x, num = 1, left = Nil, right = Nil}

sizeNode Nil = 0
sizeNode x = num x

isRed Nil = False
isRed x = color x == Red

-- rorateLeft RBTree {color = c, val = v, num = n, left = l, right = r}
--   = 

colorize c Nil = Nil
colorize c h = RBTree {color = c, val = val h, num = num h, left = left h, right = right h}

flipColors h = RBTree {color = Red, val = val h, num = num h, left = colorize Blk (left h), right = colorize Blk (right h)}

height Nil = 0
height RBTree {left = l, right = r} = 1 + max (height l) (height r)

putNode Nil = initNode


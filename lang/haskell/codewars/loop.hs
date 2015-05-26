module CanYouGetTheLoop where
import CanYouGetTheLoop.Types

{-
data Node a
instance Eq a => Eq (Node a)

next :: Node a -> Node a
-}

loop i a b = let na = next a
                 nb = next (next b)
                in if na == nb then i + 1 else loop (i + 1) na nb
len a xs = if a `elem` xs then length xs - 1 else len (next a) (a:xs)
loopSize :: Eq a => Node a -> Int
loopSize a = (loop 0 a a) - (len a [])

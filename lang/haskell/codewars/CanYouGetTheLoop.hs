-- TODO unfinished
module CanYouGetTheLoop where

-- import CanYouGetTheLoop.Types

data Node a = Node a (Node a) deriving (Eq, Show, Read)

next :: Node a -> Node a
next (Node _ n) = n

-- I don't know how to compare unlimited node
loop i a b = let na = next a
                 nb = next (next b)
                in if na == nb then i + 1 else loop (i + 1) na nb
len a xs = if a `elem` xs then length xs - 1 else len (next a) (a:xs)

loopSize :: Eq a => Node a -> Int
loopSize a = (loop 0 a a) - (len a [])

-- test code
node1 = Node 1 node2
node2 = Node 2 node3
node3 = Node 3 node4
node4 = Node 4 node2

nodex = Node 1 node2

main = print $ loopSize node1

module Main where

-- correct, but SLOW
thisSeq = concatMap (\x->[0..x]) [0..]

-- REAMIND: difference between INT , INTEGER
-- * INT is 32bits or 64bits (at least 30bits for all platform)
--   INTEGER is arbitrary precision type
-- * index of (!!) function is INT, we may need converse them to avoid
-- overflow
triSeq :: [Integer]
triSeq = map (\x-> x * (x + 1) `div` 2) [1..]

revIdx 0 = 0
revIdx k = k - (last $ takeWhile (<= k) triSeq)
-- actually, map revIdx [0..], it's yet another way of THISSEQ

pronounce xs k = xs !! fromInteger (revIdx k)

main = do
    l0 <- getLine
    l1 <- getLine
    let [_, k] = map (\w->read w::Integer) $ words l0
        ids = map (\w->read w::Integer) $ words l1
    print $ pronounce ids (k - 1)

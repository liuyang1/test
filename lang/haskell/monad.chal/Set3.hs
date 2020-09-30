{-# LANGUAGE MonadComprehensions #-}
{-# LANGUAGE RebindableSyntax #-}

module Set3 where

import MCPrelude

-- exer31
allPairs :: [a] -> [b] -> [(a,b)]
allPairs [] _ = []
allPairs (x:xs) ys = map (\y->(x,y)) ys ++ allPairs xs ys

-- exer32
data Card = Card Int String

instance Show Card where
    show (Card a s) = show a ++ s

allCards :: [Int] -> [String] -> [Card]
allCards xs ys = map (uncurry Card) $ allPairs xs ys

-- exer33
-- ([], concatMap) is monad :)
m xs f = concatMap f xs

allCombs :: (a->b->c) -> [a] -> [b] -> [c]
allCombs f xs ys = m xs (\x->
                   m ys (\y-> [f x y]))

res33 = allCombs Card cardRanks cardSuits

-- exer34
allCombs3 :: (a->b->c->d) -> [a]->[b]->[c]->[d]
allCombs3 f xs ys zs = m xs (\x->
                       m ys (\y->
                       m zs (\z-> [f x y z])))

res34 = allCombs3 (,,) [1,2] [3,4] [5,6]

-- exer35
combStep :: [a->b] -> [a] -> [b]
combStep fs xs = concatMap (\f ->
                 map (\a ->
                     f a) xs) fs

allCombs1 f xs ys = [f] `combStep` xs `combStep` ys
allCombs31 f xs ys = [f] `combStep` xs `combStep` ys `combStep` ys

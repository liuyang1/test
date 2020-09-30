{-# LANGUAGE MonadComprehensions #-}
{-# LANGUAGE RebindableSyntax #-}

module Set4 where

import MCPrelude

type Gen a = Seed -> (a, Seed)

mkGen :: a -> Gen a
mkGen a = \s -> (a, s)

generalA :: (a->b) -> Gen a -> Gen b
generalA f ga = 

repRandom :: [Gen a] -> Gen [a]
repRandom = undefined

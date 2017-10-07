module Reverse where

import Data.Function (on)
import Data.List (groupBy)

myisSpace c = c == ' '

reverseWords :: String -> String
reverseWords = concatMap reverse . splitAtSpace
    where splitAtSpace "" = []
          splitAtSpace xs = let (h, md) = span myisSpace xs
                                (m, d) = break myisSpace md
                               in h: m: splitAtSpace d


reverseWords1 = concatMap reverse . groupBy ((==) `on` (== ' '))

-- groupBy :: (a -> a -> Bool) -> [a] -> [[a]]
-- groupBy p xs
-- P = true, if a and b is same group
-- groupBy (\a b -> (a == ' ' && b == ' ') || (a /= ' ' && b /= ' '))
-- groupBy (\a b -> (a == ' ') == (b == ' '))
-- groupBy (\a b -> ((==) `on` (== ' ')) a b)
-- groupBy ((==) `on` (== ' '))
-- :)
-- smart solution!

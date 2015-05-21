module MineLocation where

import Data.Array
import Data.List
import Data.Maybe

mineLocation :: Array (Int, Int) Int -> Maybe (Int, Int)
mineLocation xx = let xs = map (elemIndex 1) xx
                      idxs = findIndex isJust xs
                   in if isNothing idxs
                         then Nothing
                         else let idx = fromJust idxs
                               in Just (idx, (fromJust (xs !! idx)))


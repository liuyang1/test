module MineLocation where

import Data.Array
import Data.List
import Data.Maybe

mineLocation :: Array (Int, Int) Int -> Maybe (Int, Int)
mineLocation xx = let rr = range (bounds xx)
                   in find (\pos -> xx ! pos == 1) rr

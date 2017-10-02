module Factor where

import Data.List
import Control.Arrow
import Prime

f 1 _ = []
f n pp@(p:ps)
    | n `mod` p == 0 = p: f (n `quot` p) pp
    | otherwise = f n ps

factor n = f n primes

factorg n = map (head &&& length) $ group fs
        where fs = factor n

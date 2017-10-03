module Factor where

import Data.List
import Control.Arrow
import Prime

f 1 _ = []
f n pp@(p:ps)
    | n `mod` p == 0 = p: f (n `quot` p) pp
    | otherwise = f n ps

factor n = f n primes

-- better solution
-- - don't depend on prime sequence
-- - save memory
factor' n = hlp n 2
    where hlp n i
              | i * i > n = [n]
              | n `mod` i == 0 = i: (hlp (n `quot` i) i)
              | otherwise = hlp n (i + 1)

factorg n = map (head &&& length) $ group fs
        where fs = factor' n

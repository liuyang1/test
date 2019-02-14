import Control.Monad (liftM2)
import Data.List (nub, sort)
import Data.Ratio ((%))

-- use fractional / instead of integer division to support more complex way
op2 a b = nub $ [a + b, a - b, b - a, a * b] ++
    (if b /= 0 then [a / b] else []) ++
    (if a /= 0 then [b / a] else [])

-- rop2 a bs = nub $ bs >>= (a `op2`)
-- lop2 as b = nub $ as >>= (`op2` b)

mop2 as bs = concat $ liftM2 op2 as bs

op3 a b c = (a `op2` b) `mop2` [c] ++
    (b `op2` c) `mop2` [a] ++
    (a `op2` c) `mop2` [b]

op4 a b c d = sort $ nub $ (op3 a b c) `mop2` [d] ++
    (op3 a b d) `mop2` [c] ++
    (op3 a c d) `mop2` [b] ++
    (op3 b c d) `mop2` [a] ++
    (op2 a b) `mop2` (op2 c d) ++
    (op2 a c) `mop2` (op2 b d) ++
    (op2 a d) `mop2` (op2 b c)

-- need 4 elements in list
-- 3,3,8,8
-- 8/(3-8/3) = 23.999.. /= 24 in Haskell
-- use Data.Ratio ((%)) instead of factional
is24 (a:b:c:d:_) = 24 `elem` op4 (a % 1) (b % 1) (c % 1) (d % 1)

comb :: [a] -> Int -> [[a]]
comb [] _ = []
comb _ 0 = [[]]
comb (x:xs) n = (map (x:) $ comb xs (n - 1)) ++ (comb xs n)

unsolvable = filter (not . is24) $ comb [1..13] 4

-- combination with duplicate elements
dcomb [] _ = []
dcomb _ 0 = [[]]
dcomb ((x,0):xs) n = dcomb xs n
dcomb ((x,p):xs) n = (map (x:) $ dcomb ((x,p-1):xs) (n - 1)) ++ dcomb xs n

-- 1820 cases
cards = map (\x -> (x, 4)) [1..13]

-- 458 unsolvable cases
unsolvable1 = filter (not . is24) $ dcomb cards 4

main = mapM_ print $ unsolvable1

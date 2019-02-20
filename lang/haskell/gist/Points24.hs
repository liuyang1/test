import           Control.Monad (ap, liftM2)
import           Data.Function (on)
import           Data.List     (groupBy, nub, sort, sortBy)
import           Data.Ratio    (denominator, numerator, (%))

-- use fractional / instead of integer division to support more complex way
op a b = nub $ [a + b, a - b, b - a, a * b] ++
    (if b /= 0 then [a / b] else []) ++
    (if a /= 0 then [b / a] else [])

-- rop2 a bs = nub $ bs >>= (a `op`)
-- lop2 as b = nub $ as >>= (`op` b)

-- op2 :: [Rational] -> [Rational] -> [Rational]
op2 as bs = concat $ liftM2 op as bs

op3 a b c = (a `op2` b) `op2` c ++
    (b `op2` c) `op2` a ++
    (a `op2` c) `op2` b

op4 a b c d = sort $ nub $ (op3 a b c) `op2` d ++
    (op3 a b d) `op2` c ++
    (op3 a c d) `op2` b ++
    (op3 b c d) `op2` a ++
    (op2 a b) `op2` (op2 c d) ++
    (op2 a c) `op2` (op2 b d) ++
    (op2 a d) `op2` (op2 b c)

-- need 4 elements in list
-- 3,3,8,8
-- 8/(3-8/3) = 23.999.. /= 24 in Haskell
is24 (a:b:c:d:_) = (24::Rational) `elem` op4 [a] [b] [c] [d]
-- use Data.Ratio ((%)) instead of factional
-- is24 (a:b:c:d:_) = 24 `elem` op4 (a % 1) (b % 1) (c % 1) (d % 1)

comb :: [a] -> Int -> [[a]]
comb [] _     = []
comb _ 0      = [[]]
comb (x:xs) n = (map (x:) $ comb xs (n - 1)) ++ (comb xs n)

-- unsolvable = filter (not . is24) $ comb [1..13] 4

-- combination with duplicate elements
dcomb [] _         = []
dcomb _ 0          = [[]]
dcomb ((x,0):xs) n = dcomb xs n
dcomb ((x,p):xs) n = (map (x:) $ dcomb ((x,p-1):xs) (n - 1)) ++ dcomb xs n

-- 1820 cases
cards = map (\x -> (x, 4)) [1..13]

-- 458 unsolvable cases
-- unsolvable1 = filter (not . is24) $ dcomb cards 4

-- main = mapM_ print $ unsolvable1

{------------------------------------------------------------------------------}

ops :: [Rational -> Rational -> Rational]
ops = [(+), (-), (*), (/), flip (-), flip (/)]

mop2 a b = nub $ ops `ap` a `ap` b


{------------------------------------------------------------------------------}
-- op with solution
data OpTree = Num Rational | Op String OpTree OpTree deriving (Show, Eq)

divS (a, sa) (b, sb) = if b /= 0 then [(a/b, ins "/" sa sb)] else []

opS (a, sa) (b, sb) = [(a + b, ins "+" sa sb),
    (a * b, ins "*" sa sb),
    (a - b, ins "-" sa sb), (b - a, ins "-" sb sa)]
    ++ divS (a, sa) (b, sb) ++ divS (b, sb) (a, sa)

-- opS (a,sa) (b,sb) = []
ins op xs ys = [Op op x y|x<-xs,y<-ys]

opS2 as bs = nub $ concat $ liftM2 opS as bs

toOp x = (x, [Num x])

-- TODO: 更好的展示方式(尽可能去除括号)
showOpT (Num n) = if denominator n == 1 then show $ numerator n else
        (show $ numerator n) ++ "/" ++ (show $ denominator n)
showOpT (Op "*" l r) = "(" ++ showOpT l ++ "*" ++ showOpT r ++ ")"
showOpT (Op "/" l r) = "(" ++ showOpT l ++ "/" ++ showOpT r ++ ")"
showOpT (Op "+" l r) = "(" ++ showOpT l ++ "+" ++ showOpT r ++ ")"
showOpT (Op "-" l r) = "(" ++ showOpT l ++ "-" ++ showOpT r ++ ")"

-- TODO: 交换律,结合律,更为聪明的化简方式
-- simplify

merge xs = map (\s -> (fst $ head s, nub $ concatMap snd s))
    $ groupBy ((==) `on` fst) $ sortBy (compare `on` fst) xs

fop3 a b c = merge $ (a `opS2` b) `opS2` c ++
    (b `opS2` c) `opS2` a ++
    (a `opS2` c) `opS2` b

fop4 a b c d = merge $ (fop3 a b c) `opS2` d ++
    (fop3 a b d) `opS2` c ++
    (fop3 a c d) `opS2` b ++
    (fop3 b c d) `opS2` a ++
    (opS2 a b) `opS2` (opS2 c d) ++
    (opS2 a c) `opS2` (opS2 b d) ++
    (opS2 a d) `opS2` (opS2 b c)

calc4 a b c d = fop4 [toOp a] [toOp b] [toOp c] [toOp d]
main = print $ map showOpT $ snd $ head $ filter (\i -> fst i == 24) $ calc4 3 3 8 8
-- ["(8/(3-8/3))"]

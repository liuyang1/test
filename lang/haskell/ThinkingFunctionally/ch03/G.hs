data Nat = Zero | Succ Nat deriving (Eq, Show, Read)

instance Ord Nat where
    compare Zero Zero = EQ
    compare Zero (Succ _) = LT
    compare (Succ _) Zero = GT
    compare (Succ m) (Succ n) = compare m n

instance Num Nat where
    m + Zero = m
    m + Succ n = Succ (m + n)
    m * Zero = Zero
    m * Succ n = m * n + m
    abs n = n -- Nat仅仅是正整数~\(≧▽≦)/~啦啦啦
    signum Zero = Zero
    signum (Succ _) = Succ Zero
    m - Zero = m
    Zero - Succ n = Zero -- m <= n, m - n = Zero强行定义的
    Succ m - Succ n = m - n
    fromInteger x
        | x <= 0 = Zero
        | otherwise = Succ (fromInteger (x - 1))

divModNat :: Nat -> Nat -> (Nat, Nat)
divModNat x y = until good improve (Zero, x)
    where good (_, b) = b < y
          improve (a, b) = (Succ a, b - y)

one = Succ Zero
two = Succ one
thr = Succ two

main = do
        print $ two + thr
        print $ thr - two
        print $ two - thr
        print $ thr * two
        print $ two == thr
        print $ two < thr
        print $ divModNat thr two
        print $ divModNat (fromIntegral 11) (fromIntegral 3)

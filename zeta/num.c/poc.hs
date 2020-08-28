toBase b = h []
    where h xs n
            | n == 0 = xs
            | otherwise = h (rem:xs) quot
                where (quot, rem) = n `divMod` b

fromBase b xs = sum $ zipWith (*) (reverse xs) [b^i | i <-[0..]]

-- This chect with Haskell big number support
convBase b0 b1 xs = toBase b1 (fromBase b0 xs)

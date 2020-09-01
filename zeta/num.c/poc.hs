-- result is from low to high
toBase :: Integral a => a -> a -> [a]
toBase b 0 = []
toBase b x = rem: toBase b quot
    where (quot, rem) = x `divMod` b

fromBase :: Integral a => a -> [a] -> a
fromBase b xs = sum $ zipWith (*) xs [b^i | i <-[0..]]

add :: Integral a => a -> [a] -> [a] -> [a]
add b [] ys = ys
add b [0] ys = ys
add b xs [] = xs
add b xs [0] = xs
add b (x:xs) (y:ys) = rem: add b [quot] (add b xs ys)
    where (quot, rem) = (x + y) `divMod` b

cmpI x y
    | x == y = 0
    | x > y = 1
    | x < y = -1

cmp [] [] = 0
cmp [] ys = -1
cmp xs [] = 1
cmp (x:xs) (y:ys)
    | c /= 0 = c
    | otherwise = cmpI x y
        where c = cmp xs ys

shift n xs = replicate n 0 ++ xs

muls b [] scl = []
muls b (x:xs) scl = rem: add b [quot] (muls b xs scl)
    where (quot, rem) = (x * scl) `divMod` b

-- O(nm)
mul :: Integral a => a -> [a] -> [a] -> [a]
mul b [] ys = []
mul b (x:xs) ys = add b (muls b ys x) (shift 1 (mul b xs ys))

-- This chect with Haskell big number support
convB b0 b1 xs = toBase b1 (fromBase b0 xs)

convBase :: Integral a => a -> a -> [a] -> [a]
convBase b0 b1 [] = []
convBase b0 b1 (x:xs) = add b1 [x] (muls b1 (convBase b0 b1 xs) b0)

dec b (0:xs) = (b-1): dec b xs
dec b (x:xs) = (x-1): xs

minus b xs [] = xs
minus b (x:xs) (y:ys)
    | x >= y = (x-y):minus b xs ys
    | otherwise = b+x-y: dec b (minus b xs ys)

-- prefer to foldr
-- a bit of slow with [] and foldr, so switch back to add3
addSeq b = foldr (add b) []
add3 b xs ys zs = add b (add b xs ys) zs

mulSimple b [] ys = []
mulSimple b xs [] = []
mulSimple b xs [y] = muls b xs y
mulSimple b [x] ys = muls b ys x
mulSimple b xs ys = add3 b ll (shift n lh) (shift (n*2) hh)
    where n = min (length xs) (length ys) `div` 2
          (xl, xh) = splitAt n xs
          (yl, yh) = splitAt n ys
          ll = mulSimple b xl yl
          hh = mulSimple b xh yh
          lh = add b (mulSimple b xl yh) (mulSimple b xh yl)

mulKara b [] ys = []
mulKara b xs [] = []
mulKara b xs [y] = muls b xs y
mulKara b [x] ys = muls b ys x
mulKara b xs ys = add3 b ll (shift n lh) (shift (n*2) hh)
    where n = min (length xs) (length ys) `div` 2
          (xl, xh) = splitAt n xs
          (yl, yh) = splitAt n ys
          ll = mulKara b xl yl
          hh = mulKara b xh yh
          lh = minus b (minus b (mulKara b (add b xl xh) (add b yl yh))
                                ll) hh

main = do
    print $ convBase 10 256 [1]
    print $ convBase 10 256 [6, 1]
    print $ convBase 10 256 [9, 6, 1]
    print $ convBase 10 256 [0, 9, 6, 1]
    print $ convBase 10 256 [2, 0, 9, 6, 1]
    print $ convBase 10 256 [3, 2, 0, 9, 6, 1]

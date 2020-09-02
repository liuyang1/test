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

-- O(nm), naive way
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
--add3 b xs ys zs = add b (add b xs ys) zs

base=256
(<+>) xs ys = add base xs ys
(<->) xs ys = minus base xs ys
(<*.>) xs scl = muls base xs scl


add3 b xs ys zs = xs <+> ys <+> ys

-- similar with Kara, but no optimization
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

-- O(n^1.5), Karastra
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

{- cheat with haskell native implementation -}
mulCheat b xs ys = toBase b ((fromBase b xs) * (fromBase b ys))

split3 n xs = (l, m, h)
    where (l, r) = splitAt n xs
          (m, h) = splitAt n r

-- TODO: support addSg, minusSg
evalToom n xs = [m0, p1, p_1, p_2, m2]
    where (m0, m1, m2) = split3 n xs
          p0 = m0 <+> m2
          p1 = p0 <+> m1
          p_1 = p0 <-> m1
          p_2 = ((p_1 <+> m2) <*.> 2) <-> m0

interpToom [r0, r1, r_1, r_2, ri] = [r0, t1, t2, t3, ri]
    where s3 = (r_2 <-> r1) <*.> (1/3)
          s1 = (r1 <-> r_1) <*.> (1/2)
          s2 = (r_1 - r0)
          t3 = ((s2 <-> s3) <*.> (1/2)) <+> (ri <*.> 2)
          t2 = s2 <+> s1 <-> ri
          t1 = s1 <-> t3

vecMul xs ys = zipWith mulToom xs ys

mulToom [] ys = []
mulToom xs [] = []
mulToom xs [y] = xs <*.> y
mulToom [x] ys = ys <*.> x
mulToom xs ys = []
    where n = min (length xs) (length ys) `div` 3
          p = evalToom n xs
          q = evalToom n xs
          r = zipWith mulToom p q

main = do
    print $ convBase 10 256 [1]
    print $ convBase 10 256 [6, 1]
    print $ convBase 10 256 [9, 6, 1]
    print $ convBase 10 256 [0, 9, 6, 1]
    print $ convBase 10 256 [2, 0, 9, 6, 1]
    print $ convBase 10 256 [3, 2, 0, 9, 6, 1]

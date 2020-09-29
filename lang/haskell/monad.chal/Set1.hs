{-# LANGUAGE MonadComprehensions #-}
{-# LANGUAGE RebindableSyntax #-}

module Set1 where

import MCPrelude

-- exer1.1
{- index sequence: [...1,0] -}
times 0 seed = ([], seed)
times n seed = (x:xs, nns)
    where (xs, ns) = times (n - 1) seed
          (x, nns) = rand ns

fiveRands :: [Integer]
fiveRands = fst $ times 5 (mkSeed 1)

-- exer1.2
randLetter :: Seed -> (Char, Seed)
randLetter seed = (toLetter x, ns)
    where (x, ns) = rand seed

randString3 :: String
randString3 = reverse $ map toLetter xs
    where (xs, _) = times 3 (mkSeed 1)

-- exer1.3
-- type synonyms
type Gen a = Seed -> (a, Seed)

generalA f s = (f x, ns)
    where (x, ns) = rand s

randEven :: Gen Integer
randEven = generalA (*2)

randOdd = generalA (\x->2*x+1)

randTen = generalA (*10)

res13 = product $ map (\f-> fst $ f (mkSeed 1)) [randEven, randOdd, randTen]

-- exer1.4
randPair :: Gen (Char, Integer)
randPair s = ((c, i), nns)
    where (c, ns) = randLetter s
          (i, nns) = rand ns

generalPair :: Gen a -> Gen b -> Gen (a, b)
generalPair fa fb s = ((a, b), nns)
    where (a, ns) = fa s
          (b, nns) = fb ns

randPair_ = generalPair randLetter rand

-- Good
generalB :: (a -> b -> c) -> Gen a -> Gen b -> Gen c
generalB f2 fa fb s = (f2 a b, nns)
    where (a, ns) = fa s
          (b, nns) = fb ns

generalPair2 = generalB (,)

randPair2 = generalPair2 randLetter rand

-- exer1.5
repRandom :: [Gen a] -> Gen [a]
repRandom [] s = ([], s)
repRandom (f:fs) s = (x:xs, nns)
    where (x, ns) = f s
          (xs, nns) = repRandom fs ns

res15 = repRandom (replicate 3 randLetter) (mkSeed 1)

-- exer1.6
genTwo :: Gen a -> (a -> Gen b) -> Gen b
genTwo fa f s = f x ns
    where (x, ns) = fa s

-- <<< it's kind of lift
mkGen :: a -> Gen a
mkGen a s = (a, s)

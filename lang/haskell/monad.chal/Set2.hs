{-# LANGUAGE MonadComprehensions #-}
{-# LANGUAGE RebindableSyntax #-}

module Set2 where

import MCPrelude

-- exer2.1
-- type
-- newtype
-- data
data Maybe a = Nil | Yes a

instance Show a => Show (Maybe a) where
    show Nil = "Nil"
    show (Yes a) = "m" ++ show a

instance Eq a => Eq (Maybe a) where
    (==) Nil Nil = True
    (==) (Yes a) (Yes b) = a == b
    (==) _ _ = False

-- exer2.2
headMay :: [a] -> Maybe a
headMay [] = Nil
headMay (x:_) = Yes x

tailMay :: [a] -> Maybe [a]
tailMay [] = Nil
tailMay (_:xs) = Yes xs

lookupMay :: Eq a => a -> [(a, b)] -> Maybe b
lookupMay _ [] = Nil
lookupMay a ((x,b):ps) = if a == x then Yes b else lookupMay a ps

divMay :: (Eq a, Fractional a) => a -> a -> Maybe a
divMay _ 0 = Nil
divMay a b = Yes (a / b)

maximumMay :: Ord a => [a] -> Maybe a
maximumMay [] = Nil
maximumMay xs = Yes $ foldr1 max xs

minimumMay :: Ord a => [a] -> Maybe a
minimumMay [] = Nil
minimumMay xs = Yes $ foldr1 min xs

-- exer2.3
fromMaybe (Yes a) = a

queryGreek xs c = let ys = lookupMay c xs in
                    if ys == Nil then Nil else
                    let h = headMay (fromMaybe ys)
                        t = tailMay (fromMaybe ys)
                        in if h == Nil || t == Nil then Nil else
                            let m = maximumMay (fromMaybe t) in
                            if m == Nil then Nil else
                            divMay (fromIntegral $ fromMaybe m) (fromIntegral $ fromMaybe h)

res23 q = zipWith (\a b -> (a, a == b)) (map (q greekDataA) xs) ys
    where xs = ["alpha", "beta", "gamma", "delta", "zeta"]
          ys = [Yes 2, Nil, Yes (10/3), Nil, Nil]

res231 q = zipWith (\a b -> (a, a == b)) (map (q greekDataB) xs) ys
    where xs = ["rho", "phi", "chi", "psi", "omega"]
          ys = [Nil, Yes (13/53), Yes (191/21), Nil, Yes 24]

test q = res23 q ++ res231 q

-- exer2.4
fm f Nil = Nil
fm f (Yes a) = f a

f2m f Nil _ = Nil
f2m f _ Nil = Nil
f2m f (Yes a) (Yes b) = f (fromIntegral a) (fromIntegral b)

queryGreek1 xs c = f2m divMay (fm maximumMay (fm tailMay ys)) (fm headMay ys)
    where ys = lookupMay c xs

chain :: (a -> Maybe b) -> (Maybe a -> Maybe b)
chain f Nil = Nil
chain f (Yes a) = f a

link :: Maybe a -> (a -> Maybe b) -> Maybe b
link Nil f = Nil
link (Yes a) f = f a

queryGreek2 xs c = f2m divMay (link (link ys tailMay) maximumMay) (link ys headMay)
    where ys = lookupMay c xs

-- exer25
salaries :: [(String,Integer)]
salaries = [("alice", 105000), ("bob", 90000), ("carol", 85000)]

addSalaries :: [(String, Integer)] -> String -> String -> Maybe Integer
addSalaries t x y = link (lookupMay x t) (\sx->
                    link (lookupMay y t) (\sy->
                        mkMaybe (sx + sy)))

mkMaybe :: a -> Maybe a
mkMaybe a = Yes a

-- It's kind of liftM2
--yLink :: (a->b->c) -> Maybe a -> Maybe b -> Maybe c
--yLink f ma mb = undefined

yLink0 :: (a->b) -> Maybe a -> Maybe b
yLink0 f ma = link ma (mkMaybe . f)

yLink f ma mb = link ma (\a->
                link mb (\b->
                    mkMaybe $ f a b))

addSalaries1 t x y = yLink (+) (lookupMay x t) (lookupMay y t)

-- exer26

tailProd :: Num a => [a] -> Maybe a
tailProd xs = yLink0 product (tailMay xs)

tailSum :: Num a => [a] -> Maybe a
tailSum xs = yLink0 sum (tailMay xs)

transMaybe :: (a->b) -> Maybe a -> Maybe b
transMaybe = yLink0

tailMax :: Ord a => [a] -> Maybe (Maybe a)
tailMax xs = maximumMay `transMaybe` (tailMay xs)

tailMin :: Ord a => [a] -> Maybe a
tailMin xs = combine $ minimumMay `transMaybe` (tailMay xs)

combine :: Maybe (Maybe a) -> Maybe a
combine Nil = Nil
combine (Yes a) = a


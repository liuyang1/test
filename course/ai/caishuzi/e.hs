{-# OPTIONS_GHC -Wall #-}
--import Data.List (tails,subsequences,permutations, unfoldr, genericLength, sort, group, nubBy)
import Data.List
import Data.Maybe
--import Data.Tuple
--import Data.Tuple.Extra (both)
import Control.Monad (liftM2)
import Control.Arrow ((&&&), first, second)

-- C(n+1,k) = C(n,k-1) + C(n,k)
comb1 :: Int -> [a] -> [[a]]
comb1 k xs
  | k == 0 = [[]]
  | length xs == k = [xs]
  | otherwise = (map ((head xs):) (comb1 (k - 1) (tail xs))) ++ (comb1 k (tail xs))
{-
comb k xs = filter (\x -> length x == k) $ subsequences xs
-}
perm k xs = concatMap permutations $ comb1 k xs

b2n True = 1
b2n False = 0
cntb = sum . map b2n
both f (x,y) = (f x, f y)

toSnd f x = (x, f x)
toFst f x = (f x, x)

groupWith f xs = groupBy (\x y->f x == f y) xs
sortWith f xs = sortBy (\x y->compare (f x) (f y)) xs

map2 [] xs = []
map2 (f:fs) xs = map f xs: map2 fs xs

ap2 f xs = map2 (map f xs) xs

count xs = map (head &&& length) $ group $ sort xs
countg xs = map (head &&& genericLength) $ group $ sort xs

nubWith f xs = nubBy (\a b->f a == f b) xs


--- specific for this problem
lst :: [[Integer]]
lst = perm 4 [0..9]

data Chk = Chk (Integer, Integer) deriving (Show, Eq, Ord)

g :: Eq a => [a] -> [a] -> Chk
g xs ys = Chk $ both cntb $ (zipWith (==) xs ys, liftM2 (==) xs ys)

data CTree a = Nil | Leaf a | Node a [(Chk, CTree a)] | Todo [(Chk, [a])] deriving (Show, Eq)

--sieve xs x = map (\xs -> (fst $ head xs, map snd xs)) $ groupWith fst $ map (toFst (g x)) xs
sieve xs x = map (fst . head &&& map snd) $ groupWith fst $ sort $ map (toFst (g x)) xs

stat xs = map (second length) xs

assoc i [] = Nothing
assoc i ((k,v):dct)
               | k == i = Just v
               | otherwise = assoc i dct

enum xs = nubWith snd $ sort$ zip xs $ map count $ (ap2 g xs)
enumg xs = nubWith snd $ sort $ zip xs $ map countg $ (ap2 g xs)


buildSub [] = []
buildSub ((c,t):xs)
                | c == Chk(4,4) = [(c, Leaf (head t))]
                | otherwise = (c, buildT t): buildSub xs

--select =
buildT [] = Nil
buildT (x:[]) = Leaf x
buildT xs = Node pivot $ buildSub $ sieve xs pivot
    where pivot = head xs

maximumWith f xs = maximumBy (\x y-> compare (f x) (f y)) xs
minimumWith f xs = minimumBy (\x y -> compare (f x) (f y)) xs

selMinMax 0 xs = head xs
selMinMax _ xs = fst $ minimumWith (maximum . map snd . snd) $ enum xs

lsq xs = sum $ map (\i->(i-m)**2) xs
    where m = realToFrac (sum xs / genericLength xs)
selLSQ 0 xs = head xs
selLSQ _ xs = fst $ minimumWith (lsq . map snd . snd) $ enumg xs

entropy xs = sum $ map (\p->(-1)*p*log(p)) $ map (/ (sum xs)) xs
selMaxE 0 xs = head xs
selMaxE _ xs = fst $ maximumWith (entropy . map snd . snd) $ enumg xs

-- map fst $ enumg xs -- get all candicates (considering symmetry)
selMinH 0 xs = head xs
selMinH _ xs = minimumWith (avgT . buildTh1 xs) $ map fst $ enumg xs

--select = selLSQ
--select = selMaxE
select = selMinH

buildSubh h [] = []
buildSubh h ((c,t):xs) = (c, buildTh (h+1) t): buildSubh h xs

buildTh h [] = Nil
buildTh h (x:[]) = Leaf x
buildTh h xs = Node pivot $ buildSubh h $ sieve xs pivot
    where pivot = select h xs

buildTh1 [] _ = Nil
buildTh1 (x:[]) _ = Leaf x
buildTh1 xs p = Node p $ buildSubh 1 $ sieve xs p

sumT h Nil = h
sumT h (Leaf _) = h
sumT h (Node _ ts) = sum $ map (sumT (h+1).snd) ts

leafT (Leaf _) = 1
leafT (Node _ ts) = sum $ map (leafT.snd) ts

midT (Leaf _) = 0
midT (Node _ ts) = (+1) $ sum $ map (midT.snd) ts

avgT t = ((sumT 1 t) - (midT t)) / (leafT t)

sam = [5,2,3,0]
samlst = [[5,2,3,0],[0,1,2,3],[4,5,6,7],[1,2,3,0], [2,3,0,1], [1,2,0,3],[2,1,3,0]]
root = sieve lst [0,1,2,3]
t01 = fromJust $ assoc (Chk (0,1)) root
t02 = fromJust $ assoc (Chk (0,2)) root
main :: IO ()
main = do
    print $ take 4 lst
    print $ g [0,1,2,3] [5,2,3,0]
    print $ Node sam [(Chk (4,4), Leaf [0,1,2,3]), (Chk (0,3), Leaf sam)]
    print $ Node sam [(Chk (4,4), Leaf sam), (Chk (0,3), (Node [0,1,2,3] [(Chk (1,3), Leaf sam)]))]
    mapM_  print $ stat $ sieve lst [0,1,2,3]
    print $ buildT samlst
    print $ sumT 1 $ buildT samlst
    print $ midT $ buildT samlst
    print $ leafT $ buildT samlst
    print $ avgT $ buildT samlst
    print $ avgT $ buildT lst
    print $ avgT $ buildTh 0 samlst
    print $ avgT $ buildTh 0 lst
    --print $ buildT lst

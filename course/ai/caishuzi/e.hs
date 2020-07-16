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
  | otherwise = map (head xs:) (comb1 (k - 1) (tail xs)) ++ comb1 k (tail xs)
{-
comb k xs = filter (\x -> length x == k) $ subsequences xs
-}

-- permutate k items from xs
perm :: Int -> [a] -> [[a]]
perm k xs = concatMap permutations $ comb1 k xs

b2n :: Num a => Bool -> a
b2n True = 1
b2n False = 0

-- count list of bool values
cntb :: [Bool] -> Integer
cntb = sum . map b2n

-- apply function to first & second element in tuple
both :: (a->b) -> (a,a) -> (b,b)
both f (x,y) = (f x, f y)

-- apply function, and combine to tuple, then we could build the group of (a,b)
toSnd :: (a->b) -> a -> (a,b)
toSnd f x = (x, f x)
toFst :: (a->b) -> a -> (b,a)
toFst f x = (f x, x)

-- "with" functions
groupWith :: Eq t => (a->t) -> [a] -> [[a]]
groupWith f = groupBy (\x y->f x == f y)
sortWith :: Ord t => (a->t) -> [a] -> [a]
sortWith f = sortBy (\x y->compare (f x) (f y))
nubWith :: Eq t => (a->t) -> [a] -> [a]
nubWith f = nubBy (\a b->f a == f b)

maximumWith :: (Ord t) => (a->t) -> [a] -> a
maximumWith f = maximumBy (\x y-> compare (f x) (f y))
minimumWith :: (Ord t) => (a->t) -> [a] -> a
minimumWith f = minimumBy (\x y -> compare (f x) (f y))

map2 [] _ = []
map2 (f:fs) xs = map f xs: map2 fs xs

-- apply bi-func to lst, get matrix of result
ap2 :: (a->a->b) -> [a] -> [[b]]
ap2 f xs = map2 (map f xs) xs

count :: Ord a => [a] -> [(a,Int)]
count xs = map (head &&& length) $ group $ sort xs
countg :: (Ord a, Num b) => [a] -> [(a,b)]
countg xs = map (head &&& genericLength) $ group $ sort xs

-- index key in dict
assoc :: Eq i => i -> [(i,a)] -> Maybe a
assoc _ [] = Nothing
assoc i ((k,v):dct)
               | k == i = Just v
               | otherwise = assoc i dct

stat :: [(k,[v])] -> [(k, Int)]
stat = map (second length)

--- specific for this problem
lst :: [[Integer]]
lst = perm 4 [0..9]

newtype Chk = Chk (Integer, Integer) deriving (Show, Eq, Ord)

-- guess function for digits4
g :: Eq a => [a] -> [a] -> Chk
g xs ys = Chk $ both cntb (zipWith (==) xs ys, liftM2 (==) xs ys)

newtype Seq = Seq [Integer] deriving (Show, Eq, Ord)

sg :: Seq -> Seq -> Chk
sg (Seq s0) (Seq s1) = g s0 s1

ssieve :: [Seq] -> Seq -> [(Chk, [Seq])]
ssieve xs x = map (fst . head &&& map snd) $ groupWith fst $ sort $ map (toFst (sg x)) xs

---


data CTree a = Nil | Leaf a | Node a [(Chk, CTree a)] | Todo [(Chk, [a])] deriving (Show, Eq)

sieve xs x = map (fst . head &&& map snd) $ groupWith fst $ sort $ map (toFst (g x)) xs

enum xs = nubWith snd $ sort$ zip xs $ map count (ap2 g xs)
enumg xs = nubWith snd $ sort $ zip xs $ map countg (ap2 g xs)

-- build sub tree
buildSub [] = []
buildSub ((c,t):xs)
                | c == Chk(4,4) = [(c, Leaf (head t))]
                | otherwise = (c, buildT t): buildSub xs

--build tree, with fixed select function head
buildT [] = Nil
buildT [x] = Leaf x
buildT xs = Node pivot $ buildSub $ sieve xs pivot
    where pivot = head xs


selHead _ xs = head xs

selMinMax 0 xs = head xs
selMinMax _ xs = fst $ minimumWith (maximum . map snd . snd) $ enum xs

lsq xs = sum $ map (\i->(i-m)**2) xs
    where m = realToFrac (sum xs / genericLength xs)
selLSQ 0 xs = head xs
selLSQ _ xs = fst $ minimumWith (lsq . map snd . snd) $ enumg xs

entropy xs = sum $ map ((\p->(-1)*p*log p) . (/ sum xs)) xs
selMaxE 0 xs = head xs
selMaxE _ xs = fst $ maximumWith (entropy . map snd . snd) $ enumg xs

-- map fst $ enumg xs -- get all candicates (considering symmetry)
selMinH 0 xs = head xs
selMinH _ xs = minimumWith (hgtT . buildTh1 xs) $ map fst $ enumg xs

--selectMinMax
--select = selLSQ
--select = selMaxE
select = selMinH

-- build tree, with contexted height
buildSubh h [] = []
buildSubh h ((c,t):xs) = (c, buildTh (h+1) t): buildSubh h xs

buildTh h [] = Nil
buildTh h [x] = Leaf x
buildTh h xs = Node pivot $ buildSubh h $ sieve xs pivot
    where pivot = select h xs


buildTh1 [] _ = Nil
buildTh1 [x] _ = Leaf x
buildTh1 xs p = Node p $ buildSubh 1 $ sieve xs p

-- build tree, with contexted height, and select function
buildSubhf f h [] = []
buildSubhf f h ((c,t):xs) = (c, buildThf f (h+1) t): buildSubhf f h xs

buildThf f h [] = Nil
buildThf f h [x] = Leaf x
buildThf f h xs = Node pivot $ buildSubhf f h $ sieve xs pivot
    where pivot = f h xs

sumT h Nil = h
sumT h (Leaf _) = h
sumT h (Node _ ts) = sum $ map (sumT (h+1).snd) ts

leafT (Leaf _) = 1
leafT (Node _ ts) = sum $ map (leafT.snd) ts

midT (Leaf _) = 0
midT (Node _ ts) = (+1) $ sum $ map (midT.snd) ts

-- height of leaf node
hgtT t = sumT 1 t - midT t

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
    print $ Node sam [(Chk (4,4), Leaf sam), (Chk (0,3), Node [0,1,2,3] [(Chk (1,3), Leaf sam)])]
    mapM_  print $ stat $ sieve lst [0,1,2,3]
    print $ buildT samlst
    print $ sumT 1 $ buildT samlst
    print $ midT $ buildT samlst
    print $ leafT $ buildT samlst
    print $ hgtT $ buildT samlst
    print $ hgtT $ buildT lst
    print $ hgtT $ buildTh 0 samlst
    print $ hgtT $ buildThf selHead 0 lst
    print $ hgtT $ buildThf selMinMax 0 lst
    --print $ buildT lst

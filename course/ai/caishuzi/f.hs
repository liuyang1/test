--{-# OPTIONS_GHC -Wall #-}
--import Data.List (tails,subsequences,permutations, unfoldr, genericLength, sort, group, nubBy)
import Data.List
import Data.Maybe
--import Data.Tuple
--import Data.Tuple.Extra (both)
import Control.Monad (liftM2)
import Control.Arrow ((&&&), first, second)
import Data.Map (Map)
import qualified Data.Map as Map

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
cntb :: [Bool] -> Int
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

map2 :: [a->b] -> [a] -> [[b]]
map2 [] _ = []
map2 (f:fs) xs = map f xs: map2 fs xs

-- apply bi-func to lst, get matrix of result
ap2 :: (a->a->b) -> [a] -> [[b]]
ap2 f xs = map2 (map f xs) xs

outerProduct :: (a->b->c) -> [a] -> [b] -> [[c]]
outerProduct f xs ys = map2 (map f xs) ys

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

-- return default value when Nothing, or inner of Just
-- fromMaybe dft (Just a) = a
-- fromMaybe dft (Nothing) = dft

stat :: [(k,[v])] -> [(k, Int)]
stat = map (second length)

-- add two list (different length)
add :: Num a => [a] -> [a] -> [a]
add [] [] = []
add xs [] = xs
add [] ys = ys
add (x:xs) (y:ys) = (x+y):add xs ys

---------------------------- specific for this problem ------
--problem = (5, 3)
--problem = (8, 3)
--problem = (8, 4)
problem = (10, 4)
len = snd problem
lst = map Seq $ perm len [0..(fst problem - 1)]
tree0 = (Chk (0,0), lst)
pivot0 = head lst

eqk4 = [[[0,1,2,3]],
        [[0,1,3,2], [0,2,3,1], [1,0,3,2], [1,2,3,0], [0,1,2,4],
         [0,1,3,4], [0,2,1,4], [0,2,3,4], [1,0,3,4], [1,2,0,4],
         [1,2,3,4], [0,1,4,5], [0,2,4,5], [1,0,4,5], [1,2,4,5],
         [1,4,3,5], [0,4,5,6], [1,4,5,6], [4,5,6,7]]]
eqk3 = [[[0,1,2]],
        [[0,2,1], [1,2,0], [0,1,3], [0,3,1],[1,2,3],
         [1,0,3], [0,3,4], [3,4,1], [3,4,0], [3,4,5]]]
up2seq = map (map Seq)
eqk = if len == 4 then up2seq eqk4 else up2seq eqk3


newtype Chk = Chk (Int, Int) deriving (Show, Eq, Ord)
-- simple for indicate guess only one case
chk0 = Chk (len, len)

-- guess function for digits4
g :: Eq a => [a] -> [a] -> Chk
g xs ys = Chk $ both cntb (zipWith (==) xs ys, liftM2 (==) xs ys)

newtype Seq = Seq [Integer] deriving (Show, Eq, Ord)

sg :: Seq -> Seq -> Chk
sg (Seq s0) (Seq s1) = g s0 s1

-- memoize solution, enumrate all input case
sgtbl = Map.fromList $ zip lst $ map (Map.fromList . zip lst) $ outerProduct sg lst lst
sgm xs ys = (sgtbl Map.! xs) Map.! ys

sg_ = sg

-- cand [Seq]  is partioned by Seq to group
sieve :: [Seq] -> Seq -> [(Chk, [Seq])]
sieve xs x = map (fst . head &&& map snd) $ groupWith fst $ sort $ map (toFst (sg_ x)) xs

-- tree with chk
-- root -> node (chk) -> node (chk)
-- As we want search pivot more scope (even not in candicates),
-- so we must record travse with specific chk result
data Treec a = Nil | Leaf Chk a | Node Chk a [Treec a] deriving (Show, Eq, Ord)

-- use one in cs, to paritation xs, then get result
-- [(part, [(Chk, count)])]
enump xs cs = nubWith snd $ sort $ zip cs $ map countg $ outerProduct sg_ cs xs

-- select first element, ignore height context
selHead _ = head

enf p = (-1)*p*log p
entropy xs = sum $ map (enf . (/ sum xs)) xs

-- when h is <=1, use eq class as candidate, otherwise, use xs
selEqk h xs = if h <= 1 then eqk !! h else xs

-- select by maximum entropy
selMaxE h xs = fst $ maximumWith (entropy . map snd . snd) $ enump xs xs

-- select by maximum entropy, with all pivots
selMaxEpk h xs = fst $ maximumWith (entropy . map snd . snd) $ enump xs (selEqk h xs)
-- filter the chk0 result, This don't affect the result
--selMaxEpk h xs = fst $ maximumWith (entropy . map snd . filter (\(a,_) -> a /= chk0) . snd) $ enump xs (selEqk h xs)

-- entropy - 2*log2*|tbbbb| as the paper suggest, with all pivots
-- This is BAD, due to wrong coeff with 2
selMaxEm 0 xs = head xs
selMaxEm h xs = fst $ maximumWith (f . snd) $ enump xs (if h == 1 then lst else xs)
    where f xs = entropy (map snd xs) - (enf $ (fromMaybe 0 $ assoc chk0 xs)*2/(sum $ map snd xs))

-- Dr. Larmouth's Strategy
selLarm h xs = fst $ minimumWith (f . snd) $ enump xs xs
    where e n = n * log n
          f xs = (sum $ map (e . snd) xs) - (fromMaybe 0 $ assoc chk0 xs) * e 2

selLarmp 0 xs = head xs
selLarmp h xs = fst $ minimumWith (f . snd) $ enump xs $ selEqk h xs
    where e n = n * log n
          f xs = (sum $ map (e . snd) xs) - fromMaybe 0 (assoc chk0 xs) * e 2

selMinH 0 xs = head xs
selMinH h xs = minimumWith (hgtT . buildTh1 selMinH (h+1) (Chk (0,0), xs)) $ map fst $ enump xs xs

selMinHp h xs = minimumWith (hgtT . buildTh1 selMinHp (h+1) (Chk (0,0), xs)) $ map fst $ enump xs $ selEqk h xs

selMinHpb h xs = minimumWith (hgtT . buildTh1 selMinHp (h+1) (Chk (0,0), xs)) $ map fst $ enump xs lst


--select :: Num h => h -> [Seq] -> Seq
select = selHead

-- build tree, with indicated pivot
buildTh1 f h (c,[x]) pivot
  | c == chk0 = Leaf c x
  | otherwise = Node c x [Leaf chk0 x]
buildTh1 f h (c,xs) pivot = Node c pivot $ map (buildThf f (h+1)) $ sieve xs pivot

-- build tree, with contexted height, and select function
buildThf :: Num h => (h->[Seq]->Seq)->h -> (Chk,[Seq]) -> Treec Seq
buildThf f h (c,xs) = buildTh1 f h (c,xs) pivot 
    where pivot = f h xs
--buildThf f h (c,[x])
--  | c == Chk (len,len) = Leaf c x
--  | otherwise = Node c x [Leaf (Chk (len,len)) x]
--buildThf f h (c,xs) = Node c pivot $ map (buildThf f (h+1)) $ sieve xs pivot
--    where pivot = f h xs

buildT = buildThf selHead 0
-- sum of height of leaf node
hgtT t = sum $ zipWith (*) (hgtStat t) [0..]

avghgt h = sum $ zipWith (*) h [0..]

hgtStat (Leaf _ _) = [1]
hgtStat (Node _ _ st) = 0: foldl add [] (map hgtStat st)

-- probe node, It's node (not leaf), It don't have leaf with same Seq
probeStat (Leaf _ _) = 0
probeStat (Node _ x st) = b2n (x `notElem` map node st) + sum (map probeStat st)

selStat (Leaf _ _) = 0
selStat (Node _ _ st) = 1 + sum (map selStat st)

statT t = (r, avghgt r, sum r, probeStat t, selStat t)
    where r = hgtStat t

node (Leaf _ x) = x
node (Node _ x _) = x

-- show tree with dot format:
-- digraph tree {
-- 1234 -> 4321 [label=00]
-- ...
-- }
showVec a chk b = showSeq a ++ " -> " ++ showSeq b ++ "[label=" ++ showChk chk ++ "]"
showSeq (Seq xs) = concatMap show xs
showChk (Chk (a,b)) = show a ++ show b
showT t = unlines $ ["digraph tree {"] ++ h pivot0 t ++ ["}"]
    where h p (Leaf c a) = [showVec p c a]
          h p (Node c a st) = showVec p c a : concatMap (h a) st

shNd n = "nd" ++ show n
shChk (Chk (a,b)) = show a ++ show b
shVec a chk b = shNd a ++ " -> " ++ shNd b ++ "[label=" ++ showChk chk ++ "]"

shSeqLabel n (Seq xs) = shNd n ++ "[label=" ++ concatMap show xs ++ "]"
shT t = unlines $ ["digraph tree {"] ++ h 0 0 t ++ ["}"]
h p n (Leaf c a)
  | c == Chk (len, len) = [shVec p c p]
  | otherwise = [shSeqLabel n a, shVec p c n]
h p n (Node c a st) = [shSeqLabel n a, shVec p c n] ++ concat ( zipWith (h n) [((n+1)*lvl)..] st)
lvl=100


t0 = buildThf selMaxEpk 0 tree0
--t0 = buildT tree0
tstShow = putStr $ shT t0

tstHeight = do
    print "test"
    print $ (hgtT &&& hgtStat) t0
    --print $ enump samlst samlst
    --print $ enump [Seq [0,1,2,3]] lst

tstSel = do
    print $ statT $ buildThf selHead 0 tree0
--    print $ statT $ buildThf selLarm 0 tree0
--    print $ statT $ buildThf selLarmp 0 tree0
--    print $ statT $ buildThf selMaxE 0 tree0
    print $ statT $ buildThf selMaxEpk 0 tree0
    print $ statT $ buildThf selMinHp 0 tree0
    print $ statT $ buildThf selMinH 0 tree0
    print $ statT $ buildThf selMinHpb 0 tree0
    --print $ buildT lst
main = tstShow

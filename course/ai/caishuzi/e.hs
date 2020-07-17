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
add [] [] = []
add xs [] = xs
add [] ys = ys
add (x:xs) (y:ys) = (x+y):add xs ys

--- specific for this problem
lst :: [[Integer]]
--lst = perm 4 [0..9]
--lst = perm 3 [0..7]
lst = perm 3 [0..4]
len = genericLength $ head $ lst

newtype Chk = Chk (Integer, Integer) deriving (Show, Eq, Ord)
chk0 (Chk (a,_)) = a

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

-- keep x in sieve self
--sieve xs x = map (fst . head &&& map snd) $ groupWith fst $ sort $ map (toFst (g x)) $ filter (/= x) xs
-- x should not in result
sieve xs x = map (fst . head &&& map snd) $ groupWith fst $ sort $ map (toFst (g x)) $ filter (/= x) xs

-- [(cand, [(Chk, count)])]
enum xs = nubWith snd $ sort$ zip xs $ map count (ap2 g xs)
enumg xs = nubWith snd $ sort $ zip xs $ map countg (ap2 g xs)

-- use one in cs, to paritation xs, then get result
enump xs cs = nubWith snd $ sort $ zip cs $ map countg $ outerProduct g cs xs

-- build sub tree
buildSub [] = []
buildSub ((c,t):xs) = (c, buildT t): buildSub xs

--build tree, with fixed select function head
buildT [] = Nil
buildT [x] = Leaf x
buildT xs = Node pivot $ buildSub $ sieve xs pivot
    where pivot = head xs

-- select first element, ignore height context
selHead _ = head

selMinMax 0 xs = head xs
selMinMax _ xs = fst $ minimumWith (maximum . map snd . snd) $ enum xs

lsq xs = sum $ map (\i->(i-m)**2) xs
    where m = realToFrac (sum xs / genericLength xs)
selLSQ 0 xs = head xs
selLSQ _ xs = fst $ minimumWith (lsq . map snd . snd) $ enumg xs

entropy xs = sum $ map ((\p->(-1)*p*log p) . (/ sum xs)) xs
selMaxE 0 xs = head xs
selMaxE _ xs = fst $ maximumWith (entropy . map snd . snd) $ enumg xs

-- entropy - 2*log*|tbbbb| as the paper suggest
selMaxEm 0 xs = head xs
selMaxEm h xs = fst $ maximumWith (f . snd) $ enump xs (if h == 1 then lst else xs)
    where f xs = (entropy $ map snd xs) - (2*(log 2)*(fromMaybe 0 $ assoc (Chk (len,len)) xs))

-- select max entropy with product on partition list & candicate list
selMaxEp 0 xs = head xs
selMaxEp h xs = fst $ maximumWith (entropy . map snd . snd) $ enump xs $ (if h == 1 then lst else xs)

-- map fst $ enumg xs -- get all candicates (considering symmetry)
selMinH 0 xs = head xs
selMinH _ xs = minimumWith (hgtT . buildTh1 xs) $ map fst $ enumg xs

selMinHp 0 xs = head xs
selMinHp h xs = minimumWith (hgtT . buildTh1 xs) $ map fst $ enump xs (if h == 1 then lst else xs)

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

-- TODO: build tree with height, function, last candidate group
-- it need refine the ap2 function, as the op0,1 is different now.

-- sum of height of leaf node
hgtT t = sum $ zipWith (*) (hgtStat t) [1..]

isLeaf (Leaf _) = True
isLeaf _ = False

hgtStat (Leaf _) = [1]
hgtStat (Node _ st) = 1: foldl add [] (map hgtStat subt)
    where subt = map snd st

node (Leaf x) = x
node (Node a _) = a

-- show tree with dot format:
-- digraph tree {
-- 1234 -> 4321 [label=00]
-- ...
-- }
showVec a b = showSeq a ++ " -> " ++ showSeq b ++ "[label=" ++ showChk (g a b) ++ "]"
showSeq = concatMap show
showChk (Chk (a,b)) = show a ++ show b
showT t = unlines $ ["digraph tree {"] ++ h t ++ ["}"]
    where h (Leaf _) = []
          h (Node a st) = map (showVec a . node) subt ++ concatMap h subt
              where subt = map snd st


sam = [5,2,3,0]
samlst = [[5,2,3,0],[0,1,2,3],[4,5,6,7],[1,2,3,0], [2,3,0,1], [1,2,0,3],[2,1,3,0]]
root = sieve lst [0,1,2,3]
t01 = fromJust $ assoc (Chk (0,1)) root
t02 = fromJust $ assoc (Chk (0,2)) root

t = buildThf selMaxEp 0 lst
tstShow = do
    --print $ showSeq [0,1,2,3]
    putStr $ showT t
    print $ t
    print $ hgtStat t
    print $ hgtT t

tstBasic :: IO ()
tstBasic = do
    print $ take 4 lst
    print $ g [0,1,2,3] [5,2,3,0]
    print $ Node sam [(Chk (4,4), Leaf [0,1,2,3]), (Chk (0,3), Leaf sam)]
    print $ Node sam [(Chk (4,4), Leaf sam), (Chk (0,3), Node [0,1,2,3] [(Chk (1,3), Leaf sam)])]
    mapM_  print $ stat $ sieve lst [0,1,2,3]
    print $ buildT samlst
    print $ enump samlst samlst
    print $ enump [[0,1,2,3]] lst
    print $ hgtStat $ buildT samlst
    print $ hgtStat $ buildTh 0 samlst
    --print $ hgtStat $ buildT lst
    --print $ buildT lst

tstSel = do
    print $ (hgtT &&& hgtStat) $ buildThf selHead 0 lst
    --print $ (hgtT &&& hgtStat) $ buildThf selMinMax 0 lst
    --print $ (hgtT &&& hgtStat) $ buildThf selLSQ 0 lst
    --print $ (hgtT &&& hgtStat) $ buildThf selMaxE 0 lst
    --print $ (hgtT &&& hgtStat) $ buildThf selMinH 0 lst
    print $ (hgtT &&& hgtStat) $ buildThf selMaxEm 0 lst
    
    print $ (hgtT &&& hgtStat) $ buildThf selMaxEp 0 lst
    print $ (hgtT &&& hgtStat) $ buildThf selMinHp 0 lst
    --print $ buildT lst
main = tstShow

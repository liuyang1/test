module Main where

import qualified Data.Vector as V
import Control.Monad hiding (mapM, liftM)
import Control.Monad.Random
import System.Random
import Data.Maybe
--------------------------------------------------------------------------------
-- Exercise 1

-- | apply pure function to monad
--
-- >>> liftM (+1) $ Just 2
-- Just 3
liftM :: Monad m => (a -> b) -> m a -> m b
liftM f mx = do
    x <- mx
    return $ f x

--------------------------------------------------------------------------------
-- Exercise 2

-- | swapV
--
-- >>> swapV 0 2 (V.fromList [1, 2, 3])
-- Just [3,2,1]
-- >>> swapV 0 2 (V.fromList [1, 2])
-- Nothing
swapV :: Int -> Int -> V.Vector a -> Maybe (V.Vector a)
swapV i j xs = do
    x <- xs V.!? i
    y <- xs V.!? j
    return $ xs V.// [(i, y), (j, x)]


-- | map a monadic function to a list
--
-- >>> mapM' Just [1..5]
-- Just [1,2,3,4,5]
-- >>> mapM' (\_ -> Nothing) [1..5]
-- Nothing
mapM' :: Monad m => (a -> m b) -> [a] -> m [b]
mapM' mf [] = return []
mapM' mf (x:xs) = do
    y <- mf x
    ys <- mapM' mf xs
    return $ y: ys

-- | get elements from list, any out of indices, return Nothing
--
-- >>> getElts [1, 3, 20] (V.fromList [0..9])
-- Nothing
-- >>> getElts [1, 3] (V.fromList [0..9])
-- Just [1,3]
getElts :: [Int] -> V.Vector a -> Maybe [a]
getElts idxs xs = mapM' (\i -> xs V.!? i) idxs

main = putStr ""


--------------------------------------------------------------------------------
-- random

type Rnd a = Rand StdGen a

-- evalRandIO :: Rnd a -> IO a
-- getRandom :: Random a => Rnd a
-- getRandomR :: Random a => (a, a) -> Rnd a

--------------------------------------------------------------------------------
-- Exercise 3

-- randomElt :: V.Vector a -> Rnd (Maybe a)
randomElt xs = do
    i <- getRandomR (0, V.length xs)
    return $ xs V.!? i

--------------------------------------------------------------------------------
-- Exercise 4

randomVec n = V.mapM (const getRandom) $ V.fromList [0..(n-1)]

randomVecR n r = V.mapM (const (getRandomR r)) $ V.fromList [0..(n-1)]

--------------------------------------------------------------------------------
-- Exercise 5

shuffleI xs 0 = return xs
shuffleI xs i = do
    j <- getRandomR (0, i)
    let ys = xs V.// [(i, xs V.! j), (j, xs V.! i)]
     in shuffleI ys (i - 1)

shuffle xs = shuffleI xs (V.length xs - 1)

--------------------------------------------------------------------------------
-- Exercise 6
-- | split to (left vector, pivot, right vector)
--
-- >>> partitionAt (V.fromList [5, 2, 8, 3, 6, 1]) 3
-- ([2,1],3,[6,5,8])
-- >>> partitionAt (V.fromList [2]) 0
-- ([],2,[])
partitionAt :: Ord a => V.Vector a -> Int -> (V.Vector a, a, V.Vector a)
partitionAt xs i = (hd, x, tl)
    where ys = if i == 0 then xs else fromJust $ swapV i 0 xs
          x = V.head ys
          yys = V.tail ys
          (hd, tl) = V.unstablePartition (< x) yys


--------------------------------------------------------------------------------
-- Exercise 7

-- | quick sort with first element as pivot
--
-- >>> qsort (V.fromList [4, 2, 3, 1])
-- [1,2,3,4]
qsort :: Ord a => V.Vector a -> V.Vector a
qsort xs = if V.null xs then xs else qsort as V.++ V.cons v (qsort bs)
    where (as, v, bs) = partitionAt xs 0

--------------------------------------------------------------------------------
-- Exercise 8
-- qsortR :: Ord a => V.Vector a -> Rnd (V.Vector a)
-- | quick sort with random element as pivot
--
-- >>> qsortR (V.fromList [4, 2, 3, 1])
-- [1,2,3,4]
qsortR xs
  | V.null xs = return xs
  | otherwise = do
      i <- getRandomR (0, V.length xs - 1)
      let (as, v, bs) = partitionAt xs i
       in do
           cs <- qsortR as
           ds <- qsortR bs
           return $ cs V.++ V.cons v ds

revK = V.fromList $ reverse [1..1000]
test = qsort revK
testR = evalRandIO $ qsortR revK

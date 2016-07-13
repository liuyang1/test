module Main where

import qualified Data.Vector as V
import Control.Monad hiding (mapM, liftM)
import Control.Monad.Random
import System.Random
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

-- randomElt :: V.Vector a -> Rnd (Maybe a)
randomElt xs = do
    i <- getRandomR (0, V.length xs)
    return $ xs V.!? i

randomVec n = V.mapM (const getRandom) $ V.fromList [0..(n-1)]

randomVecR n r = V.mapM (const (getRandomR r)) $ V.fromList [0..(n-1)]

shuffleI xs 0 = return xs
shuffleI xs i = do
    j <- getRandomR (0, i)
    let ys = xs V.// [(i, xs V.! j), (j, xs V.! i)]
     in shuffleI ys (i - 1)

shuffle xs = shuffleI xs (V.length xs - 1)

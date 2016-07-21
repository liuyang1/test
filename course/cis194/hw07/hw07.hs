{-# LANGUAGE MonadComprehensions, RecordWildCards #-}
{-# OPTIONS_GHC -Wall #-}
module Main where

import qualified Data.Vector as V
import Control.Monad hiding (mapM, liftM)
import Control.Monad.Random
import System.Random
import Data.Maybe
import Cards
import Data.Function
import Control.Applicative
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

--------------------------------------------------------------------------------
-- Exercise 9
-- select :: Ord a => Int -> V.Vector a -> Rnd (Maybe a)
-- | select rank J elem with random algo
--
-- >>> select 6 (V.fromList [0..5])
-- Just 5
-- >>> select 7 (V.fromList [0..5])
-- Nothing
select j xs
  | V.null xs = return Nothing
  | otherwise = do
      i <- getRandomR (0, V.length xs - 1)
      let (as, p, bs) = partitionAt xs i
       in case compare (V.length as + 1) j of
            GT -> select j as
            EQ -> return (Just p)
            LT -> select (j - V.length as - 1) bs


--------------------------------------------------------------------------------
-- Exercise 10
cartProdWith f a b = V.fromList $ liftM2 f (V.toList a) (V.toList b)
-- (.:) g f a b = g (f a b)
-- cartProdWith f = V.fromList .: liftM2 f `on` V.toList
-- This limit
--      cartProdWith :: (a -> a -> b) -> V.Vector a -> V.Vector a -> V.Vector b
-- however, want
--      cartProdWith :: (a -> a1 -> b) -> V.Vector a -> V.Vector a1 -> V.Vector b

-- `do` notation style
cardProdWith1 f xs ys = V.fromList $ do x <- V.toList xs
                                        y <- V.toList ys
                                        return (f x y)

-- list comprehension style
cardProdWith2 f xs ys = V.fromList [f x y | x <- V.toList xs, y <- V.toList ys]

-- applicative style
cardProdWith3 f xs ys = V.fromList $ f <$> V.toList xs <*> V.toList ys

allCards :: Deck
allCards = cartProdWith Card labels suits

newDeck :: Rnd Deck
newDeck = shuffle allCards

--------------------------------------------------------------------------------
-- Exercise 11

nextCard d
  | V.null d = Nothing
  | otherwise = Just (V.head d, V.tail d)

--------------------------------------------------------------------------------
-- Exercise 12

getCards 0 d = Just ([], d)
getCards n d = do
    (h, t) <- nextCard d
    (xs, e) <- getCards (n - 1) t
    return (h:xs, e)

-- Exercise 13 ----------------------------------------

data State = State { money :: Int, deck :: Deck }

-- {..} need RecordWildCards
-- This function from HW07.hs, material of course
repl :: State -> IO ()
repl s@State{..} | money <= 0  = putStrLn "You ran out of money!"
                 | V.null deck = deckEmpty
                 | otherwise   = do
  putStrLn $ "You have \ESC[32m$" ++ show money ++ "\ESC[0m"
  putStrLn "Would you like to play (y/n)?"
  cont <- getLine
  if cont == "n"
  then putStrLn $ "You left the casino with \ESC[32m$"
           ++ show money ++ "\ESC[0m"
  else play
    where deckEmpty = putStrLn $ "The deck is empty. You got \ESC[32m$"
                      ++ show money ++ "\ESC[0m"
          play = do
            putStrLn "How much do you want to bet?"
            amt <- read <$> getLine
            if amt < 1 || amt > money
            then play
            else do
              case getCards 2 deck of
                Just ([c1, c2], d) -> do
                  putStrLn $ "You got:\n" ++ show c1
                  putStrLn $ "I got:\n" ++ show c2
                  case () of
                    _ | c1 >  c2  -> repl $ State (money + amt) d
                      | c1 <  c2  -> repl $ State (money - amt) d
                      | otherwise -> war s{deck = d} amt
                _ -> deckEmpty
          war (State m d) amt = do
            putStrLn "War!"
            case getCards 6 d of
              Just ([c11, c21, c12, c22, c13, c23], d') -> do
                putStrLn $ "You got\n" ++ ([c11, c12, c13] >>= show)
                putStrLn $ "I got\n" ++ ([c21, c22, c23] >>= show)
                case () of
                  _ | c13 > c23 -> repl $ State (m + amt) d'
                    | c13 < c23 -> repl $ State (m - amt) d'
                    | otherwise -> war (State m d') amt
              _ -> deckEmpty

hw07 :: IO ()
hw07 = evalRandIO newDeck >>= repl . State 100

module SolveRPN where

import Data.List
import Control.Monad

-- reads :: Read a => ReadS a
-- The reads function is like read, only it returns a list with a single
-- element in case of a successful read. If it fails to read something,
-- then it returns an empty list. Apart from returning the value that it
-- read, it also returns the part of the string that it didn't consume.
-- We're going to say that it always has to consume the full input to work
-- and make it into a readMaybe function for convenience. Here it is:

-- liftM :: Monad m => (a -> r) -> m a -> m r
-- foldM :: (Monad m) => (a -> b -> m a) -> a -> [b] -> m a

readMaybe :: (Read a) => String -> Maybe a
readMaybe st = case reads st of [(x,"")] -> Just x
                                _ -> Nothing

foldingFunction :: [Double] -> String -> Maybe [Double]
foldingFunction (x:y:ys) "*" = return ((x * y):ys)
foldingFunction (x:y:ys) "+" = return ((x + y):ys)
foldingFunction (x:y:ys) "-" = return ((y - x):ys)
foldingFunction xs numberString = liftM (:xs) (readMaybe numberString)

solveRPN :: String -> Maybe Double
solveRPN st = do
    [result] <- foldM foldingFunction [] (words st)
    return result

main = do
    print $ solveRPN "1 2 * 4 + 5 *"
    print $ solveRPN "1 2 * 4 + 5 * 30" -- extra results
    print $ solveRPN "1 8 wharglbllargh" -- passing Nothing out

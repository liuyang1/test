module Main where

import System.Environment (getArgs)
import Control.Arrow ((&&&))
import Data.List (sort, group)

counter xs = map (head &&& length) $ group $ sort xs

main = do
    args <- getArgs
    s <- readFile $ head args
    let lst = read s :: [Int]
    print $ counter lst

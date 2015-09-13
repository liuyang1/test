module Main where

import System.Environment
import WordFreq(statFile)
main = do args <- getArgs
          mapM_ statFile args

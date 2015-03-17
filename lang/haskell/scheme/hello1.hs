module Main where

import System.Environment
import System.IO

main :: IO ()
main = do
    putStr "prompt> "
    hFlush stdout
    line <- getLine
    putStrLn $ "Hello, " ++ line
    main

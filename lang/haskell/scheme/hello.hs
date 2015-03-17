module Main where
import System.Environment

main :: IO ()
main = do
    args <- getArgs
    putStrLn $ "Hello, " ++ args !! 0 ++ " and " ++ args !! 1
    putStrLn $ "Arith: " ++ show ((read (args !! 0)) + (read (args !! 1)))

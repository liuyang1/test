module Main where

import System.Environment

prices = [1, 5, 8, 9, 10, 17, 17, 20, 24, 30]

cutRod0 ps n
  | n == 0 = 0
  | n > 0 = maximum $ map cut [1..(length ps)]
  | otherwise = undefined
  where cut m = if n >= m then ps !! (m - 1) + cutRod0 ps (n - m)
                         else 0

-- cutRod1 :: [Integer] -> Int -> Integer
cutRod1 ps = (map f [0 ..] !!)
    where f 0 = 0
          f n = maximum $ map (cut n) [1..(length ps)]
          cut n m = if n >= m then ps !! (m - 1) + cutRod1 ps (n - m)
                              else 0

main = do args <- getArgs
          print $ cutRod1 prices $ read (args !! 0)

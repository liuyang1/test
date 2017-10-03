import Control.Arrow

frequency :: Ord a => [a] -> [(a, Int)]
acc [] a = [(a, 1)]
acc ((b, c):xs) a
  | a == b = (a, c + 1): xs
  | a > b = (b, c): acc xs a
  | otherwise = (a, 1): (b, c): xs
frequency = foldl acc []

frequency' = map (head &&& length) . group . sort

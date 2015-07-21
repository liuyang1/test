-- only support Eq version
acc [] a = [(a, 1)]
acc ((b, c):xs) a
  | a == b = (a, c + 1): xs
  | otherwise = (b, c):  acc xs a
frequency :: Eq a => [a] -> [(a, Int)]
frequency xs = foldl acc [] xs

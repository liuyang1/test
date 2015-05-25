dive a b
  | rem a b == 0 = dive (a `div` b) b
  | otherwise = a

divl a [] = a
divl a (x:xs) = divl (dive a x) xs

n235 idx = (filter (\x -> (divl x [2,3,5]) == 1) [2..]) !! idx

module ExplosiveSum where
-- recursive method
q n m
  | n == 1 || m == 1 = 1
  | n < m = q n n
  | n == m = 1 + q n (n - 1)
  | otherwise = q n (m - 1) + q (n - m) m

memo2 f = map (\x -> map (f x) [0..]) [0..]

memoq = memo2 q

explosiveSum :: Int -> Int
explosiveSum n
  | n <= 0 = 0
  | otherwise = memoq !! n !! n

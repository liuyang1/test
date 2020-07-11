import Data.List (tails)
import Control.Monad (liftM2)

-- C(n+1,k+1) = C(n,k) + C(n-1,k) + ... + C(k,k)
comb:: Int -> [a] -> [[a]]
comb k xs
  | k == 0 = [[]]
  | length xs == k = [xs]
  | otherwise = concatMap f (tails xs)
  where f [] = []
        f (t:ts) = map (t:) (comb (k - 1) ts)

-- C(n+1,k) = C(n,k-1) + C(n,k)
comb1 :: Int -> [a] -> [[a]]
comb1 k xs
  | k == 0 = [[]]
  | length xs == k = [xs]
  | otherwise = (map ((head xs):) (comb1 (k - 1) (tail xs))) ++ (comb1 k (tail xs))

halve :: [a] -> ([a], [a])
halve xs = splitAt ((length xs) `div` 2) xs

prod :: [[a]] -> [[a]] -> [[a]]
prod xs ys = liftM2 (++) xs ys

-- C(m+n,k) = SUM[C(n,i)C(m,k-i)] 0<=i<=k
comb2 :: Int -> [a] -> [[a]]
comb2 k xs
  | k == 0 = [[]]
  | length xs == k = [xs]
  | length xs < k = []
  | otherwise = concatMap (\i -> prod (comb2 (k-i) hd) (comb2 i tl)) [0..k]
   where (hd, tl) = halve xs
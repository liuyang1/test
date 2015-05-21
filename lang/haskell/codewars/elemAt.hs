module KthElement where
import Prelude hiding ((!!))

-- | Returns Just the k-th element of the list, or Nothing if k is out of bounds.
elementAt :: Int -> [a] -> Maybe a
elementAt n [] = Nothing
elementAt n (x:xs) 
  | n <= 0 = Nothing
  | n == 1 = Just x 
  | otherwise =  elementAt (n - 1) xs

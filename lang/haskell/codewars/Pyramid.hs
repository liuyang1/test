module PyramidSlideDown where

helper up [] = maximum up
helper up (x:xs) = helper (zipWith (+) (slide up) x) xs
    where slide xs = (head xs): (zipWith (max) xs $ drop 1 xs) ++ [last xs]

longestSlideDown (x:xs) = helper x xs

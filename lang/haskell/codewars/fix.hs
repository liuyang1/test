module Fixit where
import Prelude hiding (reverse, foldr)

fix :: (a -> a) -> a
-- fix f = let x = f x in x
fix g = g (fix g)

reverse' :: ([a] -> [a]) -> [a] -> [a]
reverse' f [] = []
reverse' f (a:as) = f as ++ [a]

foldr' :: ((b -> a -> b) -> b -> [a] -> b) -> (b -> a -> b) -> b -> [a] -> b
foldr' f fn d [] = d
foldr' f fn d (x:xs) = fn (f fn d xs) x

fixrev = fix reverse'
fixfoldr = fix foldr'

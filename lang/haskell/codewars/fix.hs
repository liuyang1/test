module Fixit where

import Prelude hiding (reverse, foldr)

fix :: (func -> func) -> func
-- fix f = let yf = f yf
--             in yf
fix g = g (fix g)

-- Y combinator
-- Y(F) = F(Y(F))
-- `let` build a enviroment, which eval yf on demand

reverse' :: ([a] -> [a]) -> [a] -> [a]
reverse' f [] = []
reverse' f (x:xs) = f xs ++ [x]

foldr' :: ((b -> a -> b) -> b -> [a] -> b) -> (b -> a -> b) -> b -> [a] -> b
foldr' f fn e [] = e
foldr' f fn e (x:xs) = fn (f fn e xs) x

fact :: (Eq a, Num a) => (a -> a) -> a -> a
-- fact f 0 = 1
-- fact f n = n * f (n - 1)
fact f = \n -> if n == 0
                   then 1
                   else n * f (n - 1)

fixrev = fix reverse'
fixfoldr = fix foldr'
fixfact = fix fact

a = fixfact 5

-- a = fixfact 5
--   = (fix fact) 5
--   = (let yf = fact yf in yf) 5
--   = (fact yf) 5
--   = 5 * (yf 4)
--   = 5 * (fact yf 4)
--   = 5 * 4 * (yf 3)
--   = ...
--   = 5 * 4 * 3 * 2 * 1 * 1
--   = 120

g x = x + 1
b = (fact g) 3

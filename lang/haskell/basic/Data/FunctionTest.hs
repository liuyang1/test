module FunctionTest where

import Data.Function
import Data.Ratio ((%))

-------------------------------------------------------------------------------
-- on :: (b -> b -> c) -> (a -> b) -> a -> a -> c
-- on b u x y, apply a Binary function on results of applying an Unary
-- function of two arguments X, Y
-- it transfrom two inputs and combines the outputs
squareSum = ((+) `on` (^ 2))
-- squareSum a b = a ^ 2 + b ^ 2
-- >>> squareSum 3 4
-- 5

-------------------------------------------------------------------------------
(.:) f g x y = f (g x y)

multiply = show .: (*) `on` read
-- >>> multiply "6" "7"
-- "42"

lift f g b = f .: b `on` g

liftZS :: (Integer -> Integer -> Integer) -> (String -> String -> String)
liftZS = lift show read

mul = liftZS (*)
add = liftZS (+)
-- >>> mul "7" "6"
-- "42"
-- >>> add "7" "6"
-- "13"

-------------------------------------------------------------------------------
-- fix :: (a-> a) -> a
-- least fixed point of function, f x = x
-- define recursive function without function name
point = fix (\y n -> if n <= 1 then 1 else n * y (n-1)) 5
-- >>> point
-- 120
-- :t (\y n -> y * (n-1))
-- Num t => (t -> t) -> t -> t
-- The (t -> t) is the a in fix's type
-- :t fix (\y n -> y * (n-1))
-- Num t => t -> t

f x
  | x == 1 = 1
  | x `mod` 2 == 0 = x `div` 2
  | otherwise = 3 * x + 1
-- >>> take 30 $ iterate f 100
-- [100,50,20,76,38,19,58.29,88,44,22,11,34,17,52,26,13,40,20,10,5,16,8,4,3,2,1,1,..]

point1 = fix (\y n -> if n == 1 then 1 else (if n `mod` 2 == 0 then y $ n `div` 2 else y $ 3 * n + 1)) 100
-- 1

g x = 1 / (1 + x)
-- >>> take 10 $ iterate g (1 % 1)
-- [1 % 1, 1 % 2, 2 % 3, 3 % 5, 5 % 8, 8 % 13, ...]
-- >>> take 10 $ iterate g 1
-- [1, 0.5, 0.667, 0.6, 0.625, ..., 0.618]
point2 = fix (\y (x,t) -> if t == 0 then (x, t) else y (1/(1+x), t-1)) (1, 100)
-- >>> fst $ point2
-- 0.618033..

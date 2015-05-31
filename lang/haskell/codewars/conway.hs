module LookAndSay where
import Data.List
import Control.Arrow

digit n xs base = if n == 0 then xs else digit (n `div` base) ((n `mod` base):xs) base
digit10 n = if n == 0 then [0] else digit n [] 10
num = foldl (\x y -> 10 * x + y) 0

lookSay :: Integer -> Integer
lookSay n = num . concatMap (\x -> [toInteger (length x), head x]) . group $ digit10 n

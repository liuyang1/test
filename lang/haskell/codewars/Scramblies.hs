module Codewars.G964.Scramblies where
{-
Write function scramble(str1,str2) that returns true if a portion of str1 characters can be rearranged to match str2, otherwise returns false.

For example:
str1 is 'rkqodlw' and str2 is 'world' the output should return true.
str1 is 'cedewaraaossoqqyt' and str2 is 'codewars' should return true.
str1 is 'katas' and str2 is 'steak' should return false.

Only lower case letters will be used (a-z). No punctuation or digits will be included.
Performance needs to be considered
-}

import qualified Data.Map as Map
import Data.Function (on)

import Data.List ((\\))

m0 = Map.fromList $ zip ['a'..'z'] (repeat 0)

count :: String -> Map.Map Char Int
count = foldl (\m x -> Map.adjust (+ 1) x m) m0

(.:) :: (b -> c) -> (a -> a1 -> b) -> a -> a1 -> c
-- (.:) f g x y = f (g x y)
-- (.:) f g x = f . (g x)
-- (.:) f g = (( . ) . ( . )) f g
(.:) = ( . ) . ( . )

scramble :: String -> String -> Bool
scramble = and .: (zipWith (>=)) `on` (map snd . Map.toAscList . count)


scramble1 :: String -> String -> Bool
scramble1 s1 s2 = s2 \\ s1 == ""

test f = do
        print $ f "rkqodlw" "world"
        print $ f "cedewaraaossoqqyt" "codewars"
        print $ f "katas" "steak"

main = mapM_ test [scramble, scramble1]

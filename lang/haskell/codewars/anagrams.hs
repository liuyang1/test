module Anagram where
import Data.List

anagrams :: String -> [String] -> [String]
anagrams s = filter (\x -> sort x == sort s)

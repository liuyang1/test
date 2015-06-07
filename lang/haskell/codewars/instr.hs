module Codewars.Kata.Which where
import Data.List
inArray :: [String] -> [String] -> [String]
inArray xs ys = sort $ filter (\x -> any (isInfixOf x) ys) (nub xs)

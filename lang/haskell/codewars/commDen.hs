module Codewars.Kata.CommonDenominators where

type Ratio a = (a, a) -- Data.Ratio not suitable for this kata

convertFracs :: Integral a => [Ratio a] -> [Ratio a]
convertFracs xs = let md = foldl lcm 1 (map snd xs)
                   in map (\a -> (fst a * md `div` snd a, md)) xs

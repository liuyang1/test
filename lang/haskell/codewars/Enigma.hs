module Codewars.Kata.Enigma where

-- ref https://www.codewars.com/kata/the-enigma-machine-part-1-the-plugboard

import Data.List (nub)

-- | Either creates a processing function, or tells us that the configuration is invalid.
plugboard :: String -> Either String (Char -> Char)
plugboard xs
  | nub xs /= xs = Left "duplicate char in config"
  | odd l  = Left "invalid length config"
  | l > 20 = Left "too long config >20"
  | otherwise = Right $ wire xs
  where l = length xs

-- 将奇偶数位的字母互换
wire [] c = c
wire (x:y:xs) c
    | c == x = y
    | c == y = x
    | otherwise = wire xs c

-- only for test
a = let (Right process) = plugboard "ABCDEFGHIJKLMNOPQRST"
        in map process "ABX."

-- invalid cases
try s = let (Left r) = plugboard s in r

failures = map try ["A", "AA", ['A'..'Z']]

module Rot13 where

import Data.Char

fn c
  | c < (ord 'A') = c
  | c < (ord 'N') = c + 13
  | c <= (ord 'Z') = c - 13
  | c < (ord 'a') = c
  | c < (ord 'n') = c + 13
  | c <= (ord 'z') = c - 13
  | otherwise = c
rot13 :: String -> String
rot13 = map (chr . fn . ord)

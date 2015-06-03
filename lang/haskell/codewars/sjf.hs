module Codewars.Kata.SJF where
import Data.List

type CC    = Integer
type Job   = CC
type Index = Int

sjf :: [Job] -> Index -> CC
sjf xs i = let v = xs !! i
               (h,t) = splitAt (i + 1) xs
               hd = filter (<= v) h
               tl = filter (< v) t
            in sum hd + sum tl

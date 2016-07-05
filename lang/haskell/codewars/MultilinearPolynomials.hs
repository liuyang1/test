--------------------------------------------------------------------------------
-- Simplifying multilinear polynomials
-- https://www.codewars.com/kata/simplifying-multilinear-polynomials
-- extra handle this case, void var string like this.
--  [("",3),("",-2),("b",3),("b",4)]

-- module Codewars.Kirilloid.MultilinearPolynomials where
module MultilinearPolynomials where

import Data.List
import Data.Char
import Data.Function
import Data.Monoid

-- split string by this function. It group multi-linear var and coeff
fn x y = isDigit y || (isAlpha x && isAlpha y)
-- add extra "+" if first item have +1 coeff, which is omit, resume it back
-- ["a", "+56", "b"] -> ["+", "a", "+56", "b"]
defaultHrd xs@(x: _) = if isAlpha $ head x then "+":xs else xs

-- first compare string's length, then compare string
cmpMonomial :: String -> String -> Ordering
cmpMonomial = (compare `on` length) `mappend` compare

addDftOne xs = if isDigit $ last xs then xs else xs ++ "1"
rmDftPlus [] = []
rmDftPlus xs@(x:t) = if x == '+' then t else xs

-- When only have coeff, var is "", need resume back
-- ["+", "a", "+56"] -> [["+", "+56"], ["a", ""]]
splitCoeffVar [] = [[],[]]
splitCoeffVar [x] = [[x], [""]]
splitCoeffVar (x:y:xs) = if isAlpha $ head y
                            then let ys = splitCoeffVar xs in [x: head ys, y: ys !! 1]
                                     else let ys = splitCoeffVar (y:xs) in [x: head ys, "": ys!! 1]

splitPoly :: String -> [(Int, String)]
splitPoly xs = sortBy (cmpMonomial `on` snd) $ zip coeff' var'
    where [coeff, var] = splitCoeffVar $ defaultHrd $ groupBy fn xs
          var' = map sort var
          coeff' = map ((\x -> read x::Int) . rmDftPlus . addDftOne) coeff


accSamePoly = foldl1 (\x y -> ((fst x) + (fst y), snd x))

accPoly :: [(Int, String)] -> [(Int, String)]
accPoly = filter (\x -> (fst x /= 0)) . map accSamePoly . groupBy ((==) `on` snd)

showNum x
  | x == 1 = "+"
  | x == -1 = "-"
  | x < 0 = show x
  | otherwise = '+': show x

showPolyItem (a, "") = show a
showPolyItem (a, b) = (showNum a) ++ b
showPoly = rmDftPlus . concatMap showPolyItem

simplify :: String -> String
simplify = showPoly . accPoly . splitPoly

{-|
Module      : MultilinearPolynomials
Description : Simplifying multilinear polynomials
Copyright   : (c) Yang, 2016
Maintainer  : liuyang1
Stability   : experimental

<https://www.codewars.com/kata/simplifying-multilinear-polynomials>
-}
module MultilinearPolynomials where
-- module Codewars.Kirilloid.MultilinearPolynomials where

import Data.List
import Data.Char
import Data.Function
import Data.Monoid

-- | split string by this function. It group multi-linear var and coeff
fn x y = isDigit y || (isAlpha x && isAlpha y)
-- | add extra plug sign + if first item have +1 coeff, which is omit, resume it back
--
-- >>> defaultHrd ["a", "+56", "b"]
-- ["+","a","+56","b"]
defaultHrd xs@(x: _) = if isAlpha $ head x then "+":xs else xs

-- | first compare string's length, then compare string
-- use Monoid style
cmpMonomial :: String -> String -> Ordering
cmpMonomial = (compare `on` length) `mappend` compare

addDftOne xs = if isDigit $ last xs then xs else xs ++ "1"
rmDftPlus [] = []
rmDftPlus xs@(x:t) = if x == '+' then t else xs

-- | When only have coeff, var is void string, need resume back
--
-- >>> splitCoeffVar ["+", "a", "+56"]
-- [["+","+56"],["a",""]]
splitCoeffVar [] = [[],[]]
splitCoeffVar [x] = [[x], [""]]
splitCoeffVar (x:y:xs) = if isAlpha $ head y
                            then let ys = splitCoeffVar xs in [x: head ys, y: ys !! 1]
                                     else let ys = splitCoeffVar (y:xs) in [x: head ys, "": ys!! 1]

-- | split string to poly item
--
-- * split to poly item
--
-- * sort by cmpMonomial function
--
-- * transfer coeff to Int
splitPoly :: String -> [(Int, String)]
splitPoly xs = sortBy (cmpMonomial `on` snd) $ zip coeff' var'
    where [coeff, var] = splitCoeffVar $ defaultHrd $ groupBy fn xs
          var' = map sort var
          coeff' = map ((\x -> read x::Int) . rmDftPlus . addDftOne) coeff


accSamePoly = foldl1 (\x y -> ((fst x) + (fst y), snd x))

accPoly :: [(Int, String)] -> [(Int, String)]
accPoly = filter (\x -> (fst x /= 0)) . map accSamePoly . groupBy ((==) `on` snd)

-- | show coeff
-- omitt one, but need add extra plus sign
showCoeff x
  | x == 1 = "+"
  | x == -1 = "-"
  | x < 0 = show x
  | otherwise = '+': show x

-- ! show poly item
-- void string case, always show first, so not add extra plus sign
showPolyItem (a, "") = show a
showPolyItem (a, b) = (showCoeff a) ++ b
-- | concat show all poly item
showPoly = rmDftPlus . concatMap showPolyItem

-- | simplify multilinear polynomials
simplify :: String -> String
simplify = showPoly . accPoly . splitPoly

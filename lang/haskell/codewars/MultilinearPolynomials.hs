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

import           Data.Char
import           Data.Function
import           Data.List
import           Data.Monoid

-- | split string by this function. It group multi-linear var and coeff
-- >>> groupBy coeffOrVar "a-245cb+zyx"
-- ["a", "-245", "cb", "+", "zyx"]
coeffOrVar x y = isDigit y || (isAlpha x && isAlpha y)

isVar x = isAlpha $ head x

-- | add extra plug sign + if first item have +1 coeff, which is omit, resume it back
-- >>> decorPlus ["a", "+56", "b"]
-- ["+","a","+56","b"]
decorPlus xs@(x: _) = if isVar x then "+":xs else xs

omitPlus []      = []
omitPlus ('+':t) = t
omitPlus xs      = xs

-- | first compare string's length, then compare string
-- use Monoid style
cmpVar :: String -> String -> Ordering
cmpVar = (compare `on` length) `mappend` compare

readCoeff :: String -> Int
readCoeff = read . omitPlus . addDftOne
    where addDftOne xs = if isDigit $ last xs then xs else xs ++ "1"

-- | show coeff
-- omitt one, but need add extra plus sign
showCoeff x
  | x == 1 = "+"
  | x == -1 = "-"
  | x < 0 = show x
  | otherwise = '+': show x

-- | When only have coeff, var is empty string "", need resume back
-- >>> splitCoeffVar ["+", "a", "+56"]
-- (["+","+56"],["a",""])
splitCoeffVar [] = ([], [])
splitCoeffVar [x] = ([x], [""])
splitCoeffVar (x:y:xs) = (x: coeff, v: var)
    where (v, ls) = if isVar y then (y, xs) else ("", y:xs)
          (coeff, var) = splitCoeffVar ls

-- | split string to poly item
-- * split to poly item
-- * transfer coeff to Int
splitPoly :: String -> [(Int, String)]
splitPoly xs = zip (map readCoeff coeff) (map sort var)
    where (coeff, var) = splitCoeffVar $ decorPlus $ groupBy coeffOrVar xs

accPoly :: [(Int, String)] -> [(Int, String)]
accPoly = filter notO . map acc . groupBy ((==) `on` snd) . sortBy (cmpVar `on` snd)
    where acc = foldl1 (\x y -> (((+) `on` fst) x y, snd x))
          notO x = fst x /= 0

-- ! show poly item
-- void string case, always show first, so not add extra plus sign
showPolyItem (a, "") = show a
showPolyItem (a, b)  = showCoeff a ++ b
-- | concat show all poly item
showPoly = omitPlus . concatMap showPolyItem

-- | simplify multilinear polynomials
simplify :: String -> String
simplify = showPoly . accPoly . splitPoly

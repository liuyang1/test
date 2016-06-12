module Main where

import Data.Char
import Test.HUnit

-- 01. Volume of Cone
coneVolume :: Double -> Double -> Double
coneVolume radius height = pi * radius * radius * height / 3

-- 02. predicate strict Asscending sequence
-- MATCH is great!
asscending :: [Integer] -> Bool
asscending [] = True
asscending [_] = True
asscending (x:xs@(y:_)) = x < y && asscending xs

-- 03. square of list
squares :: [Integer] -> [Integer]
squares = map (^ 2)

-- 04. 05. parallel of resistors
-- point-free style
parallelResistors :: [Double] -> Double
parallelResistors = recip . sum . map recip

-- 06. palindrome
palindrome :: String -> Bool
palindrome xs =
    let s = map toUpper $ filter isAlphaNum xs
     in s == reverse s

{- Test Code
-}
s1 = TestCase (assertEqual "infinite squares" [1, 9, 25, 49] (take 4 (squares [1,3 ..])))

p1 = TestCase (assertBool "banana palindrome" (palindrome "Yo! Banana Boy!"))
p2 = TestCase (assertBool "carrot palindrome" (not (palindrome "Yo! Carrot Girl!")))
p3 = TestCase (assertBool "empty palindrome" (palindrome ""))
p4 = TestCase (assertBool "date palindrome" (palindrome "01/02/2010"))
p5 = TestCase (assertBool "one palindrome" (palindrome "a"))

tests = TestList [s1, p1, p2, p3, p4, p5]

run = runTestTT tests

main = run

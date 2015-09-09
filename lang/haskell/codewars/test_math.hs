module Math.Test where

import Math (calc, parse, eval)

import Data.List
import Data.Word
import Data.Char

import Test.Hspec
import Test.QuickCheck


main = hspec $ do

    describe "basic programs" $ do
        it "val" $ let s = "1" in calc s `shouldBe` 1
        it "neg" $ let s = "-3" in calc s `shouldBe` (-3)
        it "add" $ let s = "2+ 4" in calc s  `shouldBe` 6.0
        it "mul" $ let s = "2 * 4" in calc s  `shouldBe` 8.0
        it "priperty" $ let s = "3 + 2 * 4" in calc s `shouldBe` 11.0
        it "paren" $ let s = "2 / (2 + 6) + 4" in calc s `shouldBe` 4.25
        it "two paren" $ let s = "(2 / (2 + 6) * 4 ) - 6" in calc s `shouldBe` (-5)
        it "sub" $ let s = "31 -7*4-3" in calc s `shouldBe` (0.0)
    describe "neg complex" $ do
        it "neg0" $ calc "12*-1" `shouldBe` (-12)
        it "neg1" $ calc "12--1" `shouldBe` (13)

    describe "complex expressions" $ do
        it "case0" $ calc "125.45*(678 / (-2.5+ 10.5)-(80 -19) *33.25) / 20 + 11" `shouldBe` -12179.60375
        it "case1" $ calc "12* 123/-(-5 + 2)" `shouldBe` 492
        it "case2" $ calc "12* 123/(-5 + 2)" `shouldBe` -492
        it "paren" $ calc "((80 - (19)))" `shouldBe` 61
        it "paren1" $ calc "-(-(-(-4)))" `shouldBe` 4

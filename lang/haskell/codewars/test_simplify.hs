module Codewars.Kirilloid.MultilinearPolynomials.Test where

import MultilinearPolynomials (simplify)

import Test.Hspec
import Test.QuickCheck
import Test.Hspec.QuickCheck

main = hspec $ do
  describe "Sample tests" $ do
    it "Test reduction by equivalence" $ do
      simplify "dc+dcba" `shouldBe` "cd+abcd"
      simplify "2xy-yx" `shouldBe` "xy"
      simplify "-a+5ab+3a-c-2a" `shouldBe` "-c+5ab"
    it "Test monomial length ordering" $ do
      simplify "-abc+3a+2ac" `shouldBe` "3a+2ac-abc"
      simplify "xyz-xz" `shouldBe` "-xz+xyz"
    it "Test lexicographic ordering" $ do
      simplify "a+ca-ab" `shouldBe` "a-ab+ac"
      simplify "xzy+zby" `shouldBe` "byz+xyz"
    it "Test no leading +" $ do
      simplify "-y+x" `shouldBe` "x-y"
      simplify "y-x" `shouldBe` "-x+y"
    it "Test no mono" $ do
      simplify "4-5" `shouldBe` "-1"
      simplify "41-56" `shouldBe` "-15"
      simplify "3-2+3b+4b" `shouldBe` "1+7b"

module Brainfuck.Test where

import Data.List
import Data.Word
import Data.Char

import Test.Hspec
import Test.QuickCheck

import Brainfuck (executeString)

-- Codewars wraps HSpec: http://hspec.github.io/
main = hspec $ do

    describe "basic programs" $ do
        -- ok
        it "output until inclusive 0" $ let s = map (chr . fromIntegral) $ [1 :: Word8 .. 255] ++ [0] in executeString "+[,.]" s `shouldBe` Just s
        it "output n exclamation marks" $ property $ \w ->
            let plusThirtyThree = replicate (ord '!') '+'
            in (executeString (">" ++ plusThirtyThree ++ "<,[>.<-]") [chr $ fromIntegral w]) `shouldBe` Just (replicate (fromIntegral (w :: Word8)) '!')

        it "memory 0 initialized" $ executeString ".>." "" `shouldBe` Just (toChars [0, 0 :: Word8])
        it "memory operations" $ executeString ".>.+.<." "" `shouldBe` Just (toChars [0, 0, 1, 0 :: Word8])
        it "input output" $ property $ \w -> executeString ".,." [toChar w] `shouldBe` Just (toChars [0, w :: Word8])

        it "loops" $ property $ \w -> executeString "[>.<],[>.<-]" [toChar w] `shouldBe` Just (toChars $ replicate (fromIntegral w) (0 :: Word8))

    describe "complex programs" $ do
        -- ok
        it "Hello World!" $ executeString helloWorldBF "" `shouldBe` Just "Hello World!"
        it "Numbers" $ executeString numbersBF [chr 10] `shouldBe` Just "1, 1, 2, 3, 5, 8, 13, 21, 34, 55"

    describe "error handling" $ do
        it "insufficient input" $ executeString "," "" `shouldBe` Nothing

  where
    helloWorldBF = "++++++++++[>+++++++>++++++++++>+++>+<<<<-]>++.>+.+++++++..+++.>++.<<+++++++++++++++.>.+++.------.--------.>+."
    numbersBF    = ",>+>>>>++++++++++++++++++++++++++++++++++++++++++++>++++++++++++++++++++++++++++++++<<<<<<[>[>>>>>>+>+<<<<<<<-]>>>>>>>[<<<<<<<+>>>>>>>-]<[>++++++++++[-<-[>>+>+<<<-]>>>[<<<+>>>-]+<[>[-]<[-]]>[<<[>>>+<<<-]>>[-]]<<]>>>[>>+>+<<<-]>>>[<<<+>>>-]+<[>[-]<[-]]>[<<+>>[-]]<<<<<<<]>>>>>[++++++++++++++++++++++++++++++++++++++++++++++++.[-]]++++++++++<[->-<]>++++++++++++++++++++++++++++++++++++++++++++++++.[-]<<<<<<<<<<<<[>>>+>+<<<<-]>>>>[<<<<+>>>>-]<-[>>.>.<<<[-]]<<[>>+>+<<<-]>>>[<<<+>>>-]<<[<+>-]>[<+>-]<<<-]"
    toChar       = chr . fromIntegral
    toChars      = map toChar

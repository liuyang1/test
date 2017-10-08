module CmpStr where

import Data.Function
import Data.Monoid

cmpStr :: String -> String -> Ordering
cmpStr = (compare `on` length) `mappend` compare
-- first compare on length,
-- then compare raw string
-- monoid is fun and powerful! :)

main = mapM_ (print . uncurry cmpStr) xs
    where xs = [("", "")
               ,("a", "a")
               ,("a", "b")
               ,("aa", "b")
               ,("abc", "defz") -- `shouldBe` LT
               ,("abcd", "def") -- `shouldBe` GT
               ,("abc", "abc") -- `shouldBe` EQ
               ,("abc", "def") -- `shouldBe` LT
               ]

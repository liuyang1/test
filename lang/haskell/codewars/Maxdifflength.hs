import Control.Applicative
import Data.Function
import Data.Maybe

mml a = map ($ map length a) [maximum, minimum]

mxdiflg [] _ = Nothing
mxdiflg _ [] = Nothing
mxdiflg a b = Just . maximum . map abs $ (-) <$> mml a <*> mml b
-- mxdiflg a b = Just . maximum . map abs $ [((-) `on` length) x y | x <- a, y <- b]

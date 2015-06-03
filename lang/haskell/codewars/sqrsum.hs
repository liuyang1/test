module SquareSum where

squareSum :: [Integer] -> Integer
squareSum = sum . map (^2)

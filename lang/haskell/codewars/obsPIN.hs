module PIN where
import Data.Char
adj 1 = [2, 4]
adj 2 = [1, 3, 5]
adj 3 = [2, 6]
adj 4 = [1, 5, 7]
adj 5 = [2, 4, 6, 8]
adj 6 = [3, 5, 9]
adj 7 = [4, 8]
adj 8 = [5, 7, 9, 0]
adj 9 = [6, 8]
adj 0 = [8]

getPINs :: String -> [String]
getPINs xs = map (map intToDigit) $ mapM ((\x -> x: adj x) . digitToInt) xs

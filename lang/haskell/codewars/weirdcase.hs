module WeIrDStRiNgCaSe where
import Data.Char

fn x = map (\ (idx, c) -> if even idx then toUpper c else toLower c) $ zip [0..] x
toWeirdCase :: String -> String
toWeirdCase = unwords . map fn . words

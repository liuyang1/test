module RLE where
import Data.List
import Control.Arrow

runLengthEncoding :: String -> [(Int, Char)]
runLengthEncoding = map (length &&& head) . group

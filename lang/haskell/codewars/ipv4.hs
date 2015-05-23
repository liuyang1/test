module IPv4 where
import Data.Int  (Int32)
import Data.Word (Word32)
import Data.List

type IPString = String

int32ToIP :: Int32 -> IPString
todata x xs
  | length xs >= 4 = xs
  | x == 0 = todata 0 (0:xs)
  | otherwise = todata (x `div` 256) (mod x 256: xs)
int32ToIP int32 = intercalate "." $ map show $ todata int32 []

module ASCII85 where
import Data.Char
import Data.ByteString (ByteString)
import qualified Data.ByteString.Char8 as B

-- Encode
-- 1. four bytes are encoded into five ASCII85 chars(['!', 'u'])
-- 2. four bytes are taken as a single 32-bit number.
--      div by 85, add 33 to remainder of each division to get ASCII char.
-- 3. if last block less than four bytes, padding with null to four bytes.
--      after encoding, remove SAME number of char as were added for padding
-- 4. after get ASCII char, compress "!!!!!" to "z"
-- 5. finally, decorate with "<~" header and "~>" trailer.

-- other:
-- after encode, we can add whitespace and other symbol to encoded string

-- Decode
-- 1. remove decorated part
-- 2. uncompress and filter other symbol
-- 3. each block of five encoded char, multiplied by powers of 85,
--      broken to four bytes
-- 4. if last block is less than five char, then padding with 'u'.
--      after decoding, remove SAME number of char as were added for padding
digit 0 xs base = xs
digit n xs base = digit (n `div` base) ((n `mod` base): xs) base
digit85 n = digit n [] 85
digit256 n = digit n [] 256

number [] n base = n
number (b:bs) n base = number bs (base * n + b) base
number85 bs = number bs 0 85
number256 bs = number bs 0 256

chunk n [] = []
chunk n xs = (take n xs): (chunk n $ drop n xs)

rmTail len xs = take (length xs - len) xs

map2ascii n = chr (n + 33)
ascii2int c = (ord c) - 33

paddingZero n xs = replicate (n - length xs) 0 ++ xs
paddingNull n cs = cs ++ replicate (n - length cs) '\0'
paddingU n cs = cs ++ replicate (n - length cs) ('u')

digit2Int32 bs = number256 (map ord bs)
digit2Ascii cs = number85 (map ascii2int cs)

int32toAscii85 = map map2ascii . paddingZero 5 . digit85
ascii85ToInt32 = map chr . paddingZero 4 . digit256

decorAscii cs = "<~" ++ cs ++ "~>"
undecorAscii cs = take ((length cs) - 4) $ drop 2 cs

zc "!!!!!" = "z"
zc x = x
uzc '\n' = []
uzc 'z' = "!!!!!"
uzc x = if x < '!' || x > 'u' then [] else [x]

toAscii85' [] = decorAscii []
toAscii85' s = decorAscii (concatMap zc ((init ret) ++ [(rmTail padlen $ last ret)]))
  where padlen = 4 - (length (last cs))
        ret = map (int32toAscii85 . digit2Int32 . paddingNull 4) cs
        cs = chunk 4 s
toAscii85 :: ByteString -> ByteString
toAscii85 = B.pack . toAscii85' . B.unpack

fromAscii85' s = if null rs then [] else rmTail padlen $ concat ret
  where padlen = 5 - (length (last cs))
        ret = map (ascii85ToInt32 . digit2Ascii . paddingU 5) cs
        cs = chunk 5 uns
        uns = concatMap uzc rs
        rs = undecorAscii s
fromAscii85 :: ByteString -> ByteString
fromAscii85 = B.pack . fromAscii85' . B.unpack

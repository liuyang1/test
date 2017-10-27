module Main where

import System.Environment (getArgs)
import Data.List (sortOn, group)
import Control.Arrow ((&&&))
import Numeric (readHex)
-- third party lib
import qualified Data.ByteString as B
import qualified Data.ByteString.Char8 as C

linesLog = C.lines

(.&&.) f g a = (f a) && (g a)

-- Run Length Encoding
rle = map (head &&& length) . group

isInfixOf k = B.isInfixOf (C.pack k)

findSubStringTail k = B.drop (length k) . snd . B.breakSubstring (C.pack k)

findSection prefix suffix = fst . B.breakSubstring (C.pack suffix) . findSubStringTail prefix

isSHM = isInfixOf "SHM" .&&. isInfixOf "Pool: 0"
probeSHM = findSection "Size: 0x" ", Align"

process = rle . sortOn (readHex . C.unpack ) . map probeSHM . filter isSHM . linesLog

{-
1562: SHM: 1562, Pool: 1, Type: 0xdead001e, Size: 0x300000, Align: 0x10

- filter log like this kind
- get content between "Size: 0x" and ",Align"
- sort on hex
- run line encoding
-}

main = do
        args  <- getArgs
        s <- C.readFile $ head args
        mapM_ print $ process s

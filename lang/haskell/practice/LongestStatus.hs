module Main where
{- status xxx in log
find all status log, and find which status stay longest, and stay times.
-}

import System.Environment
import Data.Ord
import Data.List
import Control.Arrow
import Control.Monad
-- depend on third party lib
import qualified Data.ByteString as B
import qualified Data.ByteString.Char8 as C

-- split log to line
linesLog = C.lines

-- style 0.
-- allfilt :: [(a-> Bool)] -> (a -> Bool)
-- allfilt fns = \x -> all (\fn -> fn x) fns
--
-- isStatus = allfilt [B.isInfixOf (C.pack "buffers"),  B.isInfixOf (C.pack " status ")]

-- style 1.0.
(.&&.) f g a = (f a) && (g a)

-- style 1.1, point-free with liftM2, a little slow
-- (.&&.) = liftM2 (&&)

isStatus = B.isInfixOf (C.pack "BufferS") .&&. B.isInfixOf (C.pack " status ")

-- find status in bufferstatus line log
probeState = snd . B.breakSubstring (C.pack " status ")

-- max Run Length Encoding
maxRLE = maximumBy (comparing snd) . map (head &&& length) . group

process = maxRLE . map probeState . filter isStatus . linesLog

main = do
        args  <- getArgs
        s <- C.readFile $ head args
        print $ process s

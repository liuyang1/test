import System.Environment
import Data.Ord
import Data.List
import Data.Char
import Data.Text (breakOn, breakOnEnd, pack, unpack)
import Control.Arrow
import Control.Monad

{- status xxx in log
-- find all status log, and find which status stay longest, and stay times.
-}

-- split log to line
linesLog = lines . map toLower

-- style 0.
-- allfilt :: [(a-> Bool)] -> (a -> Bool)
-- allfilt fns = \x -> all (\fn -> fn x) fns
--
-- isStatus = allfilt [isInfixOf "buffers",  isInfixOf " status "]

-- style 1.0.
-- (.&&.) f g a = (f a) && (g a)

-- style 1.1, point-free with liftM2
(.&&.) = liftM2 (&&)

isStatus = isInfixOf "buffers" .&&. isInfixOf " status "

-- find status in bufferstatus line log
probeState = unpack . snd . breakOnEnd (pack " status ") . pack

-- max Run Length Encoding
maxRLE = maximumBy (comparing snd) . map (head &&& length) . group

process = maxRLE . map probeState . filter isStatus . linesLog

main = do
        [f, _] <- getArgs
        s <- readFile f
        print $ process s

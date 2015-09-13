module WordFreq where

--------------------------------------------------------------------------------
-- WordFreq
-- ref: http://www.scs.stanford.edu/11au-cs240h/labs/lab1.html
-- statistic on word frequency, and show it in terminal

import Data.Char
import Data.List
import Control.Arrow
import Data.Ord
import Data.Text(justifyLeft, pack, unpack)
import System.IO
import Control.Monad

stripSym x
  | last x == '.' = init x
  | last x == ',' = init x
  | last x == ')' = init x
  | head x == '(' = tail x
  | otherwise = x

breakUp ss = words $ map toLower ss

-- breakup to words, then statistic, sort as descend order
stat ss = sortBy (flip (comparing snd)) .
            map (head &&& length) . group . sort . map stripSym $ breakUp ss

-- word left justify to same width
visWord ws = map (ljust (wid + 1)) ws
  where wid = maximum $ map length ws
        ljust n s = unpack . justifyLeft n ' ' $ pack s

-- trans frequency to width, linear scale
metricFreq fs wid = map (`div` unit) fs
  where maxf = head fs
        ww = maxf `div` wid
        unit = if maxf `mod` wid /= 0 then ww + 1
                                      else ww

visual st = ret
  where (ss, tt) = unzip st
        ws = visWord ss
        -- word width + 1 + frequency bar's width == 80
        fw = 80 - length (head ws)
        ms = metricFreq tt fw
        -- not show words which frequency bar's bar is 0
        ret = filter (\x -> snd x /= 0) $ zip ws ms

visFreq n = replicate n '#'
display = mapM_ (\(v, t) -> putStrLn (v ++ visFreq t))

statFile fn = do fd <- openFile fn ReadMode
                 cnt <- hGetContents fd
                 display . visual $ stat cnt
                 hClose fd

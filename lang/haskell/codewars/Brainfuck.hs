module Brainfuck where
import Data.Char
import Data.Array
import Data.List
import Data.Maybe
import qualified Data.Map as Map

genGotoTbl [] pos posStack m = m
genGotoTbl ('[':xs) pos posStack m = genGotoTbl xs (pos + 1) (pos: posStack) m
genGotoTbl (']':xs) pos (pp:posStack) m = genGotoTbl xs (pos + 1) posStack (Map.insert pp pos . Map.insert pos pp $ m)
genGotoTbl (x:xs) pos posStack m = genGotoTbl xs (pos + 1) posStack m
gGotoTbl ins = genGotoTbl ins 0 [] Map.empty

run dp ds ip is s tbl
  | ip >= length is = Just []
  -- input
  | ins == '.' = Just (chr dd: (fromJust $ run dp ds ipn is s tbl))
  -- output
  | ins == ',' && s == [] = Nothing
  | ins == ',' = run dp (ds//[(dp, head s)]) ipn is (tail s) tbl
  -- increment data pointer
  | ins == '>' = run (dp + 1) ds ipn is s tbl
  -- decrement data pointer
  | ins == '<' = run (dp - 1) ds ipn is s tbl
  -- increment current data
  -- XXX: this style is too SLOW
  | ins == '+' = run dp (ds//[(dp, chr ((dd + 1) `mod` 256))]) ipn is s tbl
  -- decrement current data
  | ins == '-' = run dp (ds//[(dp, chr ((dd - 1) `mod` 256))]) ipn is s tbl
  -- while (current data) loop start
  | ins == '[' && dd == 0 = run dp ds (tbl Map.! ip) is s tbl
  -- while (current data) loop end, go back to loop start
  | ins == ']' && dd /= 0 = run dp ds (tbl Map.! ip) is s tbl
  | otherwise =  run dp ds ipn is s tbl
    where ins = is !! ip
          ipn = ip + 1
          dd = ord $ ds ! dp

al = 256
a = array (0, al) [(i, chr 0) | i <- [0..al]]
executeString ins input = run 0 a 0 ins input tbl
  where tbl = gGotoTbl ins

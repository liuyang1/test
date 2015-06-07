import Data.Char
import Data.Maybe
import Data.List
nrToZh '0' = '零'
nrToZh '1' = '一'
nrToZh '2' = '二'
nrToZh '3' = '三'
nrToZh '4' = '四'
nrToZh '5' = '五'
nrToZh '6' = '六'
nrToZh '7' = '七'
nrToZh '8' = '八'
nrToZh '9' = '九'
nrToZh '.' = '点'
nrlst = reverse "万千百十个"

fn '零' [] = []
fn '零' xs@('零':_) = xs
fn x acc = x: acc
removeLing = foldr fn []
removeGe = filter (/= '个')
fn0 (x,y) = if y == '零' then [y] else [y,x]
zheng ns = removeGe . removeLing . concatMap fn0 . reverse . zip nrlst . reverse $ map nrToZh ns

xiao = map nrToZh

zha n
  | n < 0 = '负': zha (-n)
  | otherwise = let s = show n
                    dotIdx = elemIndex '.' s
                 in if isJust dotIdx
                       then let (a, b) = splitAt (fromJust dotIdx) s
                             in zheng a ++ xiao b
                       else zheng s
deleteFirst a (x:xs)
  | a == x = xs
  | otherwise = x:deleteFirst a xs

simplify n
  | absn >= 10 && absn <= 19 = deleteFirst '一' $ zha n
  | otherwise = zha n
    where absn = abs n

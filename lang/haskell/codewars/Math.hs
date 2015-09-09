module Math where
--------------------------------------------------------------------------------
--  Evaluate mathematical expression
--  # support +-*/
--  # support negate op
--  # support parens
--  # use double number
--  # pass basic test (check test_math.hs)
--  very UGLY

import Data.Text(strip, pack, unpack)

lstrip = unpack . strip . pack
pstrip [] = []
pstrip xs
  | head xs == '(' && last xs == ')' = init $ tail xs
  | otherwise = xs
mstrip s = let ss = pstrip $ lstrip s in
    if ss == s then s else mstrip ss

oneOf c [] = False
oneOf c (h:cs)
  | c == h = True
  | otherwise = oneOf c cs

searchOneOf [] idx cs leftP = idx
searchOneOf (c:s) idx cs leftP
  | c == '(' = searchOneOf s ni cs (leftP + 1)
  | c == ')' = searchOneOf s ni cs (leftP - 1)
  | leftP == 0 && oneOf c cs = idx
  | otherwise = searchOneOf s ni cs leftP
  where ni = idx + 1
search s cs = searchOneOf s 0 cs 0
bsearch s cs = (length s) - 1 - search (reverse s) cs

-- This function search next operation
-- as we want support negate op,
-- which is same with minus op and higher priority than "*/", than "+-",
-- so we have to search next operation when find "-"
dsearch [] _ = -1
dsearch (c:s) idx
  | oneOf c "+-*/" = idx
  | c == ' ' = dsearch s (idx + 1)
  | otherwise = -1
ddsearch s idx0 = if ni == -1 then idx0 else idx0 - 1 - ni
  where ss = reverse $ take idx0 s
        ni = dsearch ss 0

data Expr = Op Char String String
          | Neg String
          | Val String deriving (Read, Show)

-- +-*/ op is left assoc
-- This means we have to search operation from right(back search)
-- so left operation will first evaluate when recurisve back
-- Same logic, priority is negate, */, +-.
-- So, first search +-, then */, then negate, then evaluate back
parse s
  | idx == (-1) || idx == len = Val ss
  | op == '-' && left == "" = Neg right
  | otherwise = Op op left right
  where len = length ss
        idx0 = bsearch ss "+-"
        idx0' = if idx0 == -1 || ss !! idx0 /= '-'
                    then idx0
                    else ddsearch ss idx0
        idx1 = bsearch ss "*/"
        idx = if idx0' == (-1) || idx0' == len then idx1 else idx0'
        op = ss !! idx
        left = mstrip $ take idx ss
        right = mstrip $ drop (idx + 1) ss
        ss = mstrip s

eval (Val s) = read s :: Double
eval (Neg s) = (-1) * (eval (parse s))
eval (Op op left right)
  | op == '+' = l + r
  | op == '-' = l - r
  | op == '*' = l * r
  | op == '/' = l / r
  where l = (eval (parse left))
        r = (eval (parse right))

calc = eval . parse

module RPN where
import Data.Char

rpn [] ss = head ss
rpn (x:xs) ss
  | isDigit hx = rpn xs (read x: ss)
  | '+' == hx = rpn xs (h1 + h0: ts)
  | '-' == hx = rpn xs (h1 - h0: ts)
  | '*' == hx = rpn xs (h1 * h0: ts)
  | '/' == hx = rpn xs (h1 / h0: ts)
    where hx = head x
          h0 = head ss
          h1 = ss !! 1
          ts = drop 2 ss

calc :: String -> Double
calc s = let ss = words s
          in if null ss then 0 else rpn ss []

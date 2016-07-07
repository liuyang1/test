-- module Codewars.Exercise.MergeChecker where
module MergeChecker where

-- | isMerge
--
-- >>> isMerge "" "" ""
-- True
-- >>> isMerge "code" "code" ""
-- True
-- >>> isMerge "code" "codf" ""
-- False
-- >>> isMerge "codewars" "code" "wars"
-- True
-- >>> isMerge "codewars" "cdw" "oears"
-- True
-- >>> isMerge "codewars" "cod" "wars"
-- False
-- >>> isMerge "Bananas from Bahamas" "Bahas" "Bananas from am"
-- True
isMerge :: String -> String -> String -> Bool
isMerge [] [] [] = True
isMerge [] _ _ = False
isMerge s [] y = s == y
isMerge s x [] = s == x
isMerge (s:ss) xx@(x:xs) yy@(y:ys)
  | s == x && s == y = isMerge ss xs yy || isMerge ss xx ys
  | s == x = isMerge ss xs yy
  | s == y = isMerge ss xx ys
  | otherwise = False

isMerge' (s:ss) xx@(x:xs) yy(y:ys) = s == x && isMerge ss xs yy || s == y && isMerge ss xx ys
isMerge' s x y = s == x ++ y

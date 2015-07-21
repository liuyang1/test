helper xs n nl nr
-- final status
  | nl == nr && n == nl = [xs]
-- if have all left, then add right
  | n == nl = helper (xs ++ ")") n nl (nr + 1)
-- if left right balanced, then start add left
  | nl == nr = (helper (xs ++ "(") n (nl + 1) nr)
-- otherwise, add left or add right
  | otherwise = (helper (xs ++ "(") n (nl + 1) nr) ++ (helper (xs ++ ")") n nl (nr + 1))
balancedParens :: Int -> [String]
balancedParens n = helper "" n 0 0

bracket x = "(" ++ x ++ ")"

balancedParens' 0 = [""]
balancedParens' n = [bracket x ++ rest | m <- [0..n - 1], x <- balancedParens' m, rest <- balancedParens' (n - m - 1)]

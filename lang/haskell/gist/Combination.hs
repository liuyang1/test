-- C(m, n) = C(m - 1, n) + C(m - 1, n - 1)
combination m n
  | n == 1 = map (: []) [0..(m - 1)]
  | m == n = [[0..(m - 1)]]
  | otherwise = combination (m - 1) n ++
      map (\xs -> xs ++ [m - 1]) (combination (m - 1) (n - 1))

combs xxs@(x:xs) n
  | n == 1 = map (: []) xxs
  | n == length xxs = [xxs]
  | otherwise = combs xs n ++ map (x:) (combs xs (n - 1))

sort [] = []
sort (x: xs) = sort [y | y <- xs, y < x] ++ [x] ++ sort [y | y <- xs, y >= x]

main = print $ sort [1, 4, 2, 7, 4]

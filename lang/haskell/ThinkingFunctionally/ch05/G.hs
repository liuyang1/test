minimum0 xs = foldl1 min xs

main = print $ minimum0 [1, 4, 2, -1, 10]

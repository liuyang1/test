-- ref: https://stackoverflow.com/questions/21265454/subsequences-of-length-n-from-list-performance
comba [] = [[[]]]
comba (x:xs) = let next = comba xs
               in zipWith (++) ([]:next)
                               (map (map (x:)) next ++ [[]])

comb n xs = let l = length xs in if n>l then [] else comba xs !! (l-n)

comb_naive 0 _ = [[]]
comb_naive _ [] = []
comb_naive n (x:xs) = map (x:) (comb_naive (n-1) xs) ++ comb_naive n xs

-- profile with :set +s

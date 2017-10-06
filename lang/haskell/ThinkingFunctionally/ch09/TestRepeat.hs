-- slow one
repeat0 x = x: repeat0 x

-- fast one
repeat1 x = xs
    where xs = x: xs

main = do
        print $ last $ take 10000000 $ repeat0 1
        print $ last $ take 10000000 $ repeat1 1

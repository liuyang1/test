ret0 = [1,2] >>= \n -> ['a','b'] >>= \ch -> return (n,ch)
ret1 = [(n, ch) | n <- [1, 2], ch <- "ab"]
ret2 = do
        n <- [1, 2]
        ch <- "ab"
        return (n, ch)

main = mapM_ print [ret0, ret1, ret2]

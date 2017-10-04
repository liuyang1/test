first :: (a -> Bool) -> [a] -> Maybe a
first p xs = if null ys then Nothing else Just (head ys)
                        where ys = filter p xs

main = print $ first (> pi) [1..]

f1 x = case x of
           Nothing -> Nothing
           Just v -> Just v

-- 写出 Maybe a -> Maybe a的函数
-- 不是很理解题意，把答案抄过来:(

-- unhandled on Nothing case, ghc throw Non-exhaustive pattern in case
f2 x = case x of
           Just v -> Just v

-- handle Nothing case, but cannot handle undefined exception
f3 x = case x of
           Nothing -> Just undefined
           Just v -> Just v

-- handle Nothing case, but cannot handle undefined exception
f4 x = Just (case x of
            Nothing -> undefined
            Just v -> v)

try f = map f [Nothing, Just 1]

test = map try [f1, f4, f3, f2]

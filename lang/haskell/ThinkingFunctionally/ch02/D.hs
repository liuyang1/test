-- Beaver style
-- if Beaver evaluate Susan's style, it have to O(length(xs)) times to filter first.
first0 p xs | null xs = error "Empty list"
            | p x = x
            | otherwise = first0 p (tail xs)
            where x = head xs

-- Susan style
first1 p = head . filter p

main = do
        print $ first0 (> pi) [1..]
        print $ first1 (> pi) [1..]

-- eager求值策略的时候，必须显式递归定义，而不能利用map和filter函数

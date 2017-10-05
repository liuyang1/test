takeWhile0 _ [] = []
takeWhile0 p (x:xs)
    | p x = x: takeWhile0 p xs
    | otherwise = []

dropWhile0 _ [] = []
dropWhile0 p (x:xs)
    | p x = dropWhile0 p xs
    | otherwise = x: xs

whiteSpace :: Char -> Bool
whiteSpace c = c `elem` [' ', '\t', '\n']

span0 p xs = (takeWhile0 p xs, dropWhile0 p xs)

words0 :: String -> [String]
words0 xs = w: words ws
    where ys = dropWhile0 whiteSpace xs
          (w, ws) = span0 (not . whiteSpace) xs

main = print $ words " Hello, \tWorld!\n"


-- :type 'H'
-- 'H' :: Char
-- :type "H"
-- "H" :: String
-- :type 2001
-- 2001 :: Num a => a
-- 2001 is a kind of Num, including Integer, Fractional, ..
-- :type "2001"
-- "2001" :: String

a = [1, 2, 3] ++ [3, 2, 1] == [1, 2, 3, 3, 2, 1]
b = "Hello" ++ "World!" == "HelloWorld!"
c = [1, 2, 3] ++ [] == [1, 2, 3]
d = "Hello" ++ "" ++ "World!" == "HelloWorld!"

main = print $ and [a, b, c, d]

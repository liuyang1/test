-- 1,
-- toLower, toUpper

-- 2,
-- 如果有可能也是
-- words . unwords == id
-- 成立
-- unwords 固定的插入一个空格， 而words是去除尽可能多的空格

-- 3,
-- x:xs
import Data.Char (toUpper)

modernise :: String -> String
modernise = unwords . map captital . words
    where captital (x:xs) = toUpper x: xs

main = print $ modernise "the morphology of prex- an essay in meta-algorithmics"

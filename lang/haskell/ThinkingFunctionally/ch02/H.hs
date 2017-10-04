import Data.Char (isDigit)

type CIN = String

toDigit :: Int -> Char
toDigit x = head $ show x

-- read return Read class 类族
-- 这里限定为Int
getDigit :: Char -> Int
getDigit c = read [c]

-- 检查addSum的输入
sanity xs = length xs == 8 && all isDigit xs

-- xs 的有效输出是8个digit数字
addSum :: CIN -> CIN
addSum xs = xs ++ ((showFix 2 '0') . sum . map getDigit) xs

-- show number but fixed width with leading c
showFix :: Int -> Char -> Int -> String
showFix n c x = (replicate (n - length s) c) ++ s
    where s = show x

valid :: CIN -> Bool
valid xs = length xs == 10 && (sum $ map getDigit hd) == read tl
    where (hd, tl) = splitAt 8 xs

main = do
        print $ valid $ addSum "00000012"
        print $ valid $ addSum "63245134"
        print $ sanity "00000012"
        print $ sanity "63245134"
        print $ False == sanity "00"
        print $ False == sanity "1234567o"

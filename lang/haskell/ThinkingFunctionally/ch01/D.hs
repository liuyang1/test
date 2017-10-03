import Data.Char

s = "Hello World"

-- 转换为小写字母，然后分词
f0 = words . map toLower

-- 分词，然后转换为小写字母
f1 = map (map toLower) . words

main = print $ f0 s == f1 s

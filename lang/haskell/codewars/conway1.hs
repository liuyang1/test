import Data.List

-- skillful use READ & SHOW
lookSay :: Integer -> Integer
lookSay n = read . concatMap (\xs -> show (length xs) ++  [head xs]) . group $ show n

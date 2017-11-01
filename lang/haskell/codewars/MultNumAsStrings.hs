module MultNumAsStrings where

import Data.Function (on)

-- (.:) = ( . ) . ( . )
(.:) = fmap fmap fmap

-- | multiply two numbers as strings
multiply :: String -> String -> String
multiply = show .: (*) `on` read

main = do
    print $ multiply "123" "456"
    print $ multiply "123123749801723487" "456123841234"

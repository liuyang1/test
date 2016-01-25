foo0 :: Maybe String
foo0 = Just 3   >>= (\x ->
       Just "!" >>= (\y ->
       Just (show x ++ y)))

-- DO notation
foo1 :: Maybe String
foo1 = do
        x <- Just 3
        y <- Just "!"
        Just (show x ++ y)

main = do print foo0
          print foo1

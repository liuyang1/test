-- fmap :: (a -> b) -> f a -> f b
-- NOTICE: this F is not a function, but a FUNCTOR
{- instance Functor IO where
    fmap f action = do
        result <- action
        return (f result)
-}
-- fmap (String -> String) -> IO String -> IO String
main = do line <- fmap reverse getLine
          putStrLn $ "You said " ++ line ++ " backwards!"
          putStrLn $ "Yes, you really said" ++ line ++ " backwards!"

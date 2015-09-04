module Person where

data Person = Person { name :: String }

greet :: Person -> String -> String
greet person otherName =
  "Hi " ++ otherName ++ ", my name is " ++ (name person)

module SimpleTokeniser (Token(..), tokenize) where

data Token = Token String | Brackets [Token]
    deriving (Eq, Show)

operatorChars :: String
operatorChars = "!#$%&*+-/<=>@^_.,;"

findNextBkt [] _ _ = -1
findNextBkt ('(': xs) cnt idx = findNextBkt xs (cnt + 1) (idx + 1)
findNextBkt (')': xs) 0 idx = idx
findNextBkt (')': xs) cnt idx = findNextBkt xs (cnt - 1) (idx + 1)
findNextBkt (_: xs) cnt idx = findNextBkt xs cnt (idx + 1)

-- tokenize :: String -> Maybe [Token]
tokenize ('(': xs) = tokenize inner
  where i = findNextBkt xs 0 0
        (inner, outer) = splitAt i xs

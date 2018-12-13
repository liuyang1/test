module RegExpParser
       ( RegExp(..)
       , parseRegExp
       ) where

data RegExp = Normal Char       -- ^ A character that is not in "()*|."
            | Any               -- ^ Any charater
            | ZeroOrMore RegExp -- ^ Zero or more occurances of the same regexp
            | Or RegExp RegExp  -- ^ A choice between 2 regexps
            | Str [RegExp]      -- ^ A sequence of regexps.
  deriving (Show, Eq)


parseRegExp :: String -> Maybe RegExp
parseRegExp '.':s = Just Any

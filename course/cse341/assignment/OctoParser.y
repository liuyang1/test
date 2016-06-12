{- IF YOU TURN THIS FILE IN, PLEASE WRITE YOUR NAME AND SECTION HERE -}

{- CSE 341, Autumn 2015.  Definition of a parser for Octopus for
use in the Octopus interpreter. -}

{
module OctoParser 
where
import Data.Char
}

%name octoparse
%tokentype { Token }
%error { parseError }

%token 
      integer         { TokenInt $$ }
      boolean         { TokenBool $$ }
      atom            { TokenAtom $$ }
      '('             { TokenLeftParen }
      ')'             { TokenRightParen }
      '\''            { TokenQuote }
%%
      
OctoValue : '(' OctoValues ')'    { OctoList $2 }
         | atom                 { OctoSymbol $1 }
         | integer              { OctoInt $1 }
         | boolean              { OctoBool $1 }
         | '\'' OctoValue        { OctoList [OctoSymbol "quote", $2] }

OctoValues : {- empty -}            { [] }
          | OctoValue OctoValues     { $1 : $2 }

{
parseError :: [Token] -> a
parseError _ = error "Parse error"

-- An environment is a list of (name,value) pairs.  The name will
-- be an OctoSymbol (although the type declaration doesn't capture this).
type Environment = [(OctoValue,OctoValue)]

{- Declarations of the datatype for Octopus data.  The constructors
used in data produced by the parser are OctoInt (Octopus integers),
OctoBool (Octopus booleans), OctoSymbol (Octopus symbols, or atoms),
and OctoList (lists).  The remaining 2 types, OctoClosure and
OctoPrimitive, are not actually used by the parser, just the
interpreter.-}

data OctoValue
      = OctoInt Int
      | OctoBool Bool
      | OctoSymbol String
      | OctoList [OctoValue]
      | OctoClosure [OctoValue] Environment OctoValue
      | OctoPrimitive String
      deriving (Show, Eq)

data Token
      = TokenInt Int
      | TokenBool Bool
      | TokenAtom String
      | TokenLeftParen
      | TokenRightParen
      | TokenQuote
      deriving (Show, Eq)

-- a lexer to take the input string and break it into a list of tokens

lexer :: String -> [Token]
lexer [] = []
lexer (c:cs) 
      | isSpace c = lexer cs
      | isDigit c = lexNum (c:cs)
      | isRacketAtomStartChar c = lexAtom (c:cs)
lexer ('(':cs)  = TokenLeftParen : lexer cs
lexer (')':cs)  = TokenRightParen : lexer cs
lexer ('\'':cs) = TokenQuote : lexer cs

lexNum cs = TokenInt (read num) : lexer rest
       where (num,rest) = span isDigit cs

-- lexAtom looks for a symbol.  But we also need to handle #t and #f, and
-- also the special case of an integer +3 or -5 (both legal in Racket)
lexAtom cs = result : lexer rest
    where 
      (t:ts,rest) = span isRacketAtomChar cs
      result = lexAtomHelper t ts

lexAtomHelper t ts
    | (t=='+' || t=='-') && all isDigit ts  && not (null ts) = 
        TokenInt (read ts * (if t=='-' then -1 else 1))
    | t=='#' && ts=="t" = TokenBool True
    | t=='#' && ts=="f" = TokenBool False
    | otherwise = TokenAtom $ (t:ts)

isRacketAtomStartChar c = isAlpha c || elem c "!#$%&|*+-/:<=>?@^_~"
isRacketAtomChar c = isRacketAtomStartChar c || isDigit c

parse = octoparse . lexer

}



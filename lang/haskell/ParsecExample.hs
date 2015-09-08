module ParsecExample where
--------------------------------------------------------------------------------
--  Ref: http://stackoverflow.com/questions/26537189/parsec-get-buildexpressionparser-example-to-typecheck-in-2014
--  Simple Parsec Example
--  ghc 7.10.1
--  parsec 2015-09-08

import Text.Parsec
import Text.Parsec.Expr
import Text.Parsec.Token

import Text.Parsec.Language (javaStyle)

lexer   = makeTokenParser javaStyle

expr    = buildExpressionParser table term
          <?> "expression"

term    =  parens lexer expr <|> natural lexer
           <?> "simple expression"

table   = [ [prefix "-" negate, prefix "+" id ]
          , [postfix "++" (+1)]
          , [binary "*" (*) AssocLeft, binary "/" (div) AssocLeft ]
          , [binary "+" (+) AssocLeft, binary "-" (-)   AssocLeft ]
          ]

binary  name fun assoc = Infix (do{ reservedOp lexer name; return fun }) assoc
prefix  name fun       = Prefix (do{ reservedOp lexer name; return fun })
postfix name fun       = Postfix (do{ reservedOp lexer name; return fun })

runExample s = parseTest expr s

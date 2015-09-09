module Main where

import Text.ParserCombinators.Parsec hiding (spaces)
import System.Environment
import Control.Monad
import Numeric

symbol :: Parser Char
symbol = oneOf "!#$%&|*+-/:<=>?@^_~"

spaces :: Parser()
spaces = skipMany1 space

data LispVal = Atom String
             | List [LispVal]
             | DottedList [LispVal] LispVal
             | Number Integer
             | String String
             | Bool Bool

parseString :: Parser LispVal
parseString = do char '"'
                 x <- many $ (escaped <|> noneOf "\"")
                 char '"'
                 return $ String x
  where escaped = char '\\' >> choice (zipWith escapedChar codes reps)
        escapedChar code rep = char code >> return rep
        codes = ['b', 'n', 'f', 'r', 't', '\\', '\"', '/']
        reps = ['\b', '\n', '\f', '\r', '\t', '\\', '\"', '/']
-- Exercise 1.1.2, 1.1.3 escpaed char

parseAtom :: Parser LispVal
parseAtom = do first <- letter <|> symbol
               rest <- many (letter <|> digit <|> symbol)
               let atom = first:rest
               return $ case atom of
                          "#t" -> Bool True
                          "#f" -> Bool False
                          _    -> Atom atom

parseNumber :: Parser LispVal
-- parseNumber = liftM (Number . read) $ many1 digit

-- Exercise 1.1.1
-- parseNumber = do num <- many1 digit
--                  return . Number $ read num
-- parseNumber = (many1 digit) >>= (return . Number . read)

parseNumber = parseDec <|> parsePrefix
  where parseDec = (many1 digit) >>= (return . Number . read)
        parsePrefix = do char '#'
                         ptype <- oneOf "xod"
                         case ptype of
                           'x' -> (many1 hexDigit) >>= (return . Number . nstrip . readHex)
                           'o' -> (many1 octDigit) >>= (return . Number . nstrip . readOct)
                           'd' -> (many1 digit) >>= (return . Number . read)
                           where nstrip = fst . head
-- Exercise 1.1.4
-- radix prefix number with Numeric lib

parseExpr :: Parser LispVal
parseExpr = parseNumber <|> parseAtom <|> parseString

showVal :: LispVal -> String
showVal (String str) = "\'" ++ str ++ "\'"
showVal (Atom name) = name
showVal (Number num) = show num
showVal (Bool True) = "#t"
showVal (Bool False) = "#f"

instance Show LispVal where show = showVal

readExpr :: String -> String
readExpr input = case parse parseExpr "lisp" input of
  Left err -> "No match: " ++ show err
  Right val -> "Found " ++ show val

main :: IO ()
main = do (expr:_) <- getArgs
          putStrLn (readExpr expr)

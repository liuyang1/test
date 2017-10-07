module MoleculeToAtoms where

import Data.Char
import Data.List

-- function about bracket
brkt = [('{', '}'), ('[', ']'), ('(', ')')]
isLeftBrkt c = c `elem` map fst brkt
isRightBrkt c = c `elem` map snd brkt
isMatch c0 c1 = any (\(a, b) -> c0 == a && c1 == b) brkt

-- return content in bracket, and trailing string
findNextBrkt :: String -> Either String (String, String)
findNextBrkt (x:xs) = hlp xs "" [x]
    where hlp "" _ _ = Left "Mismatched parenthesis"
          hlp (x: xs) acc stk
              | isAlphaNum x = hlp xs (x:acc) stk
              | isLeftBrkt x = hlp xs (x:acc) (x:stk)
              | isRightBrkt x && isMatch (head stk) x = if null (tail stk)
                                                           then Right (reverse acc, xs)
                                                           else hlp xs (x:acc) (tail stk)
              | otherwise = Left "Mismatched parenthesis"


-- return number and trailing string
eatNum :: String -> (Int, String)
eatNum xs = (if null n then 1 else read n, s)
    where (n, s) = span isDigit xs

mulGroup :: Int -> [(String, Int)] -> [(String, Int)]
mulGroup m = map (\(s, n) -> (s, n * m))

-- combine same molecule number, keep original order
accu :: [(String, Int)] -> [(String, Int)]
accu [] = []
accu ((c, n):xs) = (c, n + m): accu other
    where (same, other) = partition (\(c1, _) -> c == c1) xs
          m = sum $ map snd same

parseMoleculeI, parseMolecule :: String -> Either String [(String,Int)]

parseMoleculeI "" = Right []
parseMoleculeI xx@(x: xs)
  | isLeftBrkt x = do
      (i, o) <- findNextBrkt xx
      let (n, t) = eatNum o
      ii <- parseMoleculeI i
      ys <- parseMoleculeI t
      return (mulGroup n ii ++ ys)
  | isUpper x = let (i, o) = span isLower xs
                    (n, t) = eatNum o
                 in do
                     ys <- parseMoleculeI t
                     Right ((x:i, n): ys)
  | otherwise = Left "Not a valid molecule"

parseMolecule xs = do
    r <- parseMoleculeI xs
    return (accu r)

-- a0 = parseMolecule "(OH)2(ABC)123(Ab)"
-- a1 = parseMolecule "(OH)2A1BC"
-- a2 = parseMolecule "(OH(AB)3)2(ABC)123"
-- a3 = parseMolecule "O2"
-- a4 = parseMolecule "H2O"
-- a5 = parseMolecule "Mg(OH)2"
-- a6 = parseMolecule "K4[ON(SO3)2]2"
-- a7 = parseMolecule "pie"
-- v0 = findNextBrkt "(abcdef)aa"
-- v1 = findNextBrkt "(ab()ef)aa"
-- v2 = findNextBrkt "(a(())b()ef)aa"
-- v3 = findNextBrkt "(a((])b()ef)aa"
-- v4 = findNextBrkt "a]))"
-- a8 = parseMolecule "Mg(OH"
-- a9 = parseMolecule "{[Co(NH3)4(OH)2]3Co}(SO4)3"

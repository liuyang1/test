import Data.Char

--------------------------------------------------------------------------------
-- validate credit card number

toDigits :: Int -> [Int]
toDigits x
  | x <= 0 = []
  | otherwise = map digitToInt $ show x

toDigitsRev = reverse . toDigits

doubleEveryThis [] = []
doubleEveryThis [x] = [x]
doubleEveryThis (x: y: xs) = x: 2 * y: doubleEveryThis xs
doubleEveryOther = reverse . doubleEveryThis . reverse

sumDigits = sum . concatMap toDigits

validate = (\x -> x `mod` 10 == 0) . sumDigits . doubleEveryOther . toDigits


--------------------------------------------------------------------------------
-- Tower of Hanoi
-- move disc from first to SECOND

-- type synonym
type Peg = String
type Move = (Peg, Peg)
-- hanoi :: Int -> Peg -> Peg -> Peg -> [Move]

hanoi n a b c
  | n <= 0 = []
  | n == 1 = [(a, b)]
  | otherwise = hanoi (n - 1) a c b ++ hanoi 1 a b c ++ hanoi (n - 1) c b a

--------------------------------------------------------------------------------
-- Tower of Hanoi 4
-- Exercise 1.17 in Graham, Knuth, and Patashnik, Concrete Mathematics,
-- second ed., Addison-Wesley, 1994
-- Reve's puzzle, still open problem
-- Ref: https://www.wikiwand.com/en/Tower_of_Hanoi
--
-- move disc from first to LAST
hanoi4 0 _ _ _ _ = []
hanoi4 1 a _ _ d = [(a, d)]
hanoi4 2 a b _ d = [(a, b), (a, d), (b, d)]
hanoi4 n a b c d
  | n <= 0 = []
  | otherwise = hanoi4 x a c d b ++ hanoi y a d c ++ hanoi4 x b c a d
  where y = takelast n
        x = n - y

takelasti n i = if (n <= i) then n else takelasti (n - i) (i + 1)
takelast n = takelasti n 1

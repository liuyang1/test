import Data.Char (isAlpha, toLower)
import Data.Bool (bool)

isPalind s = x == reverse x
        where x = map toLower $ filter isAlpha s

verify = all isPalind ["Madam, I'm Adam", "Doc, note I dissent. A fast never prevents a fatness. I diet on cod."]

palindrome = do
        print "Enter a string:"
        s <- getLine
        putStrLn (if isPalind s then "Yes!" else "No!")
        -- putStrLn (bool "No!" "Yes!" (isPalind s))
        palindrome

main = palindrome

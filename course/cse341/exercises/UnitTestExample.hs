{- CSE 341, Haskell
Example of using the HUnit unit test framework.  See
    http://hackage.haskell.org/package/HUnit
for additional documentation.

To run the tests type "runTestTT tests" at the Haskell prompt.  I find it
convenient to give a shorter name to this, so that (as in the program
below) you can just type "run".
-}

module UnitTestExample
    where

import Test.HUnit
import Data.Char

{- Two useful functions in the HUnit package are assertEqual and assertBool.
The arguments to 'assertEqual' are:
      a descriptive string
      the expected value
      the value being tested
The arguments to 'assertBool' are:
      a descriptive string
      the boolean value being tested
-}

-- test1, test2, and test3 succeed
-- test4 doesn't give the expected answer
-- test5 gets an error trying to evaluate the thing being tested
test1 = TestCase (assertEqual "arithmetic test" 7 (3+4)) 
test2 = TestCase (assertEqual "string test" "squid clam" ("squid " ++ "clam")) 
test3 = TestCase (assertBool "char test" (isUpper 'A'))
test4 = TestCase (assertEqual "bad arithmetic" 8 (3+4))
test5 = TestCase (assertEqual "bad list" [3] (tail []))
tests = TestList [TestLabel "test1" test1
                  , TestLabel "sea creature test" test2
                  , TestLabel "char  test" test3
                  -- TestLabel "test4" test4,
                  -- TestLabel "test5" test5
                  ]

-- shortcut to run the tests
run = runTestTT tests

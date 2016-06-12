{- PLEASE WRITE YOUR NAME AND SECTION HERE ON THE VERSION YOU TURN IN -}

{- Starter file for CSE 341, Autumn 2015, Haskell Project:
   the Octopus Interpreter -}
 

module OctopusInterpreter
    where

import OctoParser
import Data.Char
import Data.Maybe
import Test.HUnit

{- The heart of the interpreter is the eval function, which takes an
OctoValue and evaluates it in the given environment.  As in Racket,
expressions are represented as lists.  The eval function is defined by
cases.

The environment is a list of (name,value) pairs.  The type
'Environment' is a synonym for [(OctoValue,OctoValue)], and is defined in
OctoParser.  To look up a name the interpreter searches the environment
starting from the front, so that one variable can shadow another. -}

eval :: OctoValue -> Environment -> OctoValue

-- integers and booleans evaluate to themselves
eval (OctoInt i) env = OctoInt i
eval (OctoBool b) env = OctoBool b

{- To evaluate a symbol, look it up in the current environment and 
return the value found; or raise an exception if it's not found. -}
eval s@(OctoSymbol v) env = case lookup s env of
    Nothing -> error ("name not found: " ++ v)
    Just r -> r

-- A quoted expression evaluates to that expression.
eval (OctoList [OctoSymbol "quote", x]) env = x

{- An expression starting with (lambda ...) evaluates to a closure,
where a closure consists of a list of variable names (OctoSymbols),
the environment of definition, and the body. -}
eval (OctoList [OctoSymbol "lambda", OctoList vars, body]) env = 
    OctoClosure vars env body

{- If we don't match any of the special cases, the first thing in the
list should evaluate to a function.  Apply it to its arguments.  There
are two cases: either the function is a user-defined function, or a
primitive.  These are handled separately.  In either case, the
arguments are found by evaluating each of the expressions after the
function name in the current environment. -}
eval (OctoList (f:xs)) env = case eval f env of
    c@(OctoClosure _ _ _) -> apply c args
    (OctoPrimitive p) -> fromJust (lookup p primitives) $ args
    where  args = map (\x -> eval x env) xs

{- Apply a user-defined function to the given arguments.  The user-defined
function has already been evaluated to get an OctoClosure, and the arguments
have already been evaluated as well in the calling environment.
Then make a new environment by extending the environment of definition
of the lambda (which is part of the closure).  In the extended
environment, the actual args are bound to the respective formal names,
evaluate the body of the function in this new environment, and return
the result. -}
apply (OctoClosure vars f_env body) args = error "TO BE WRITTEN"


-- list of primitive functions and their definitions in Haskell
-- for the starter, we only have + ... you need to add various other functions
primitives = [ ("+",octoplus)
               , ("-",octominus)
               , ("*",octotimes)
               , ("cons", octocons)
               , ("car", octocar)
               , ("cdr", octocdr)
             ]

-- helper function for arithmetic functions (if we defined OctoInt using
-- the {... } syntax we wouldn't need this, but I didn't feel like cluttering
-- everything else up)
getint (OctoInt i) = i

octowrap :: ([Int] -> Int) -> [OctoValue] -> OctoValue
octowrap fn = OctoInt . fn . map getint

-- The octoplus function takes a list of OctoInts and adds them.
octoplus = octowrap sum

minus [x] = -1 * x
minus (x:xs) = x - (sum xs)

octominus = octowrap minus

octotimes = octowrap product

octocons [x, OctoList xs] = OctoList (x: xs)

octocar [OctoList ls] = head ls

octocdr [OctoList ls] = OctoList (tail ls)
-- the global enviroment has null?, and the primitives 
-- (and 'not' after you add it) 
global_env = [
  (OctoSymbol "null?", eval (parse "(lambda (x) (equal? x '()))") global_env)]
   ++ map (\(name,fn) -> (OctoSymbol name, OctoPrimitive name)) primitives
{- null? is defined by evaluating the result of parsing the lambda.
Notice that the environment in which it is evaluated is the global
environment, which we are defining.  This may look circular, but it
works fine in Haskell due to lazy evaluation.  You won't be able to
actually use null? until you have lambda working, but again because
Haskell is lazy it's fine to put the definition in from the start. -}

-- for the unit tests, make a test environment by extending the global env
testenv = [(OctoSymbol "k", OctoInt 5) , (OctoSymbol "s" , OctoSymbol "x")]
   ++ global_env

evparse s = eval (parse s) testenv

{- unit tests for the interpreter -}
{- the tests that don't work yet with the starter program are commented out -}

-- Define a helper function to make test cases so that they aren't so
-- ridiculously verbose
testeval expr val = TestLabel 
  ("evaluating " ++ expr ++ " should give " ++ show val)
  (TestCase (assertEqual "an OctoTest" val (evparse expr)))
 
-- note that we need to get the white space right for the input strings
-- for this, since we are testing for string equality on the output
show_test_cases = ["7", "#f", "(1 2 3)", "(+ 1 (* 2 3))", "'squid", "'(1 2 3)"]

-- some expressions to test let
shadowing_let1 = " \
\  (let ((k (+ 3 k))  \
\        (j k))       \
\    (+ j k))"

nested_let1 = " \
\  (let ((x 3)       \
\        (y 4))      \
\    (let ((x 100))  \
\      (+ x y)))"

nested_let2 = " \
\  (let ((x 3)       \
\        (y 4))      \
\    (let ((x (+ x y)))  \
\      (+ x y)))"

nested_let3 = " \
\  (let ((n 10))                     \
\    (let ((f (lambda (x) (+ x n)))  \
\          (n 3))                    \
\      (+ (f 100) n)))"

{- Expr to test that lambda is closing over its environment of
definition.  Here, n is defined in the let but not where f is used --
so we would get an error if the body of the lambda were evaluated in
the wrong environment.-}
let_test_closure = "                         \
\ (let ((y 10)                               \
\       (f (let ((n 50))                     \
\            (lambda (x) (+ x (* 2 n))))))   \
\   (f y))"

-- the factorial function
letrec_fact =  " \
\ (letrec       \
\    ((fact (lambda (n) (if (equal? 0 n) 1 (* n (fact (- n 1)))))))   \
\  (fact 4))"

-- the range function returns a list of integers from n down to 0
letrec_range =  " \
\ (letrec       \
\    ((range (lambda (n) (if (equal? 0 n) '() (cons n (range (- n 1)))))))   \
\  (range 4))"

letrec_map =  " \
\ (letrec \
\    ((map (lambda (f s) \
\        (if (null? s) '() (cons (f (car s)) (map f (cdr s)))))))  \
\  (map (lambda (n) (* n 2)) '(10 20 30)))"

-- Curried version of map. Here map itself takes one argument (a function), 
-- and returns a new function that maps over a list.
letrec_curried_map =  "                                                     \
\ (letrec                                                                   \
\    ((map (lambda (f)                                                      \
\            (lambda (s)                                                    \
\              (if (null? s) '() (cons (f (car s)) ((map f) (cdr s))))))))  \
\  ((map (lambda (n) (* n 2))) '(10 20 30)))"

-- test mutual recursion in letrec with a ridiculously slow version of
-- the mod2 function
letrec_mod2 = "                                                 \
\ (letrec                                                       \
\     ((zero? (lambda (x) (equal? x 0)))                        \
\      (even? (lambda (x) (if (zero? x) #t (odd? (- x 1)))))    \
\      (odd?  (lambda (x) (if (zero? x) #f (even? (- x 1)))))   \
\      (mod2 (lambda (x) (if (even? x) 0 1))))                   \
\   (cons (mod2 5) (cons (mod2 6) '())))"


tests = TestList [
  testeval "#t" (OctoBool True),
  testeval "#f" (OctoBool False),
  testeval "3" (OctoInt 3),
  testeval "'x" (OctoSymbol "x"),
  testeval "'(x 5)" (OctoList [OctoSymbol "x", OctoInt 5]),
  testeval "k" (OctoInt 5),
  testeval "s" (OctoSymbol "x"),
  -- test + for 0, 1, 2, and 4 args
  testeval "(+)" (OctoInt 0),
  testeval "(+ 3)" (OctoInt 3),
  testeval "(+ 3 4)" (OctoInt 7),
  testeval "(+ 3 4 10 20)" (OctoInt 37),
  testeval "(- 4 3)" (OctoInt 1),
  testeval "(- 3 4 3)" (OctoInt (-4)),
  testeval "(- 3)" (OctoInt (-3)),
  testeval "(* 4)" (OctoInt 4),
  testeval "(* 3 4 5)" (OctoInt 60),
  testeval "(cons 1 '(2 3))" (OctoList [OctoInt 1, OctoInt 2, OctoInt 3]),
  testeval "(cons 1 '())" (OctoList [OctoInt 1]),
  testeval "(car '(1 2 3))" (OctoInt 1),
  testeval "(cdr '(1 2 3))" (OctoList [OctoInt 2, OctoInt 3]),
  -- can't use the shortcut for these -- testing octoshow
--  TestLabel "octoshow" (TestCase (assertEqual "test octoshow" 
--    show_test_cases (map (octoshow . parse) show_test_cases))),
--  TestLabel "octoshow primitive" (TestCase (assertEqual "test octoshow" 
--    "<primitive *>" (octoshow $ evparse "*"))),
--  testeval "( (lambda (x) x) 7)" (OctoInt 7),
--  testeval "((lambda (x y) (+ x (+ y 10))) 3 4)" (OctoInt 17),
  -- the inner lambda's y should shadow the outer one, so we get 11 
  -- rather than 3
--  testeval "( (lambda (x y) ((lambda (y) (+ x y)) 10)) 1 2)" (OctoInt 11),
--  testeval "(let ((x 3)) (+ x 4))" (OctoInt 7),
--  testeval "(let ((x 3) (y 4)) (+ x y))" (OctoInt 7),
--  testeval shadowing_let1 (OctoInt 13),
--  testeval nested_let1 (OctoInt 104),
--  testeval nested_let2 (OctoInt 11),
--  testeval nested_let3 (OctoInt 113),
--  testeval let_test_closure (OctoInt 110),
  -- The first two if cases have a nonexistant variable on the branch not 
  -- taken. If we evaluated it we would get an error, so if this works it
  -- means 'if' is not evaluating the branch not taken.
  -- The third checks that anything other than #f counts as true.
  -- The fourth makes sure the test is evaluated (it evaluates to #f).
--  testeval "(if #t 3 bad)" (OctoInt 3),
--  testeval "(if #f bad (+ 2 3))" (OctoInt 5),
--  testeval "(if 2 3 5)" (OctoInt 3),
--  testeval "(if (equal? k 10) (+ 2 3) (+ 10 20))" (OctoInt 30),
  -- cond
--  testeval "(cond (else (+ 2 3)))" (OctoInt 5),
--  testeval "(cond (#t (+ 10 10)) (else (+ 2 3)))" (OctoInt 20),
--  testeval "(cond (#f bad) (else (+ 2 3)))" (OctoInt 5),
--  testeval "(cond ((equal? 1 2) bad) (#f bad) (else (+ 2 3)))" (OctoInt 5),
--  testeval "(cond (#f bad) (#t 88) (else (+ 2 3)))" (OctoInt 88),
  -- bind a new name to a primitive and try it
--  testeval "(let ((f +)) (f 3 4))" (OctoInt 7),
  -- rebind * (!!!).  This is a very weird thing to do, but it should work
--  testeval "(let ((* +)) (* 3 4))" (OctoInt 7),
--  testeval "(eval '(+ 2 3))" (OctoInt 5),
  -- more complex eval example -- make sure the argument to eval is evaluated
  -- in the current environment (here with x bound to 10)
--  testeval "(let ((x 10)) (eval (cons '+ (cons x (cons 5 '())))))" 
--    (OctoInt 15),
  -- another complex eval example -- make sure eval evaluates its expression
  -- in the global environment.  To do this, (yuck) rebind * and make sure the 
  -- expression still uses the global * 
  -- (If you don't believe this is legal, try pasting the part between 
  -- the " marks into racket and evaluating it.)
--  testeval "(let ((* null?)) (eval (cons '* (cons 3 (cons 5 '())))))"
--    (OctoInt 15),
  -- Recursive function tests
--  testeval letrec_fact (OctoInt 24),
--  testeval letrec_range (evparse "'(4 3 2 1)"),
--  testeval letrec_map (evparse "'(20 40 60)"),
--  testeval letrec_curried_map (evparse "'(20 40 60)"),
--  testeval letrec_mod2 (evparse "'(1 0)"),
  -- tests for the string extra credit question
--  testeval "(string-append)" (OctoString ""),
--  testeval "(string-append \"fish \" \"clam \" \"squid\" )" 
--    (OctoString "fish clam squid"),
  -- a final dummy test to avoid problems with the last comma!
  testeval "3" (OctoInt 3)
  ]

run = runTestTT tests 

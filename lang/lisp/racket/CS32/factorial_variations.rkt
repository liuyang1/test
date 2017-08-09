#lang racket

;;; File: factorial_variations.rkt
;;; Author: Robert R. Snapp (rsnapp@uvm.edu)
;;; Created on October 21, 2013
;;;
;;; Title: "Variations on the factorial design pattern."
;;;
;;; The following functions demonstrate how a diverse set of functions can be
;;; based on a simple recursive pattern: the one used to compute n! (spoken as
;;; "n factorial"):
;;;
;;; We are all familiar with factorial. Here it is written using one of the helper functions
;;; that we just defined.

(define (factorial n)
  (if (zero? n)
      1
      (* n (factorial (sub1 n)))))

;;; Note that zero? is a built-in function that returns #t only if its argument is numerically
;;; equal to 0. Otherwise it returns false.

;;; Variation 1, is a silly function. It creates a list of n apples:
(define (apples n)
  (if (zero? n)
      '()
      (cons 'apple (apples (sub1 n)))))

;;; Verify that (apples 3) -> (apple apple apple). What about (apples (factorial 7))?

;;; Variation 2: counting the number of items in a list (also known as the length function):
(define (count s)
  (if (null? s)
      0
      (add1 (count (cdr s)))))

;;; Verify that (count (apples (factorial 7))) -> 5040.

;;; Variation 3: get the last item in a list that contains at least one element.
(define (last-not-null s)
  (if (null? (cdr s))
      (car s)
      (last-not-null (cdr s))))

;;; Verify that (last-not-null '(a b c)) -> c

;;; Variation 4: get the last item in an arbitrary list. If the list is empty () is returned.

(define (last s)
  (if (null? s)
      '()
      (last-not-null s)))

;;; What is the difference between (last '()) and (last '(()))? Can you think of a better
;;; definition for last.

;;; Variation 5: a tool for selecting the i-th item in the list s.
;;; Note that i better not exceed (count s).
(define (select i s)
  (if (zero? i)
      (car s)
      (select (sub1 i) (cdr s))))

;;; Verify that (select 4 '(a b c d e f)) -> e. What about (select 0 '(a b c))? or
;;; (select 4 '(a b c))?

;;; Apples are dull. Let's combine what we have to make a basket of fruit.

(define (fruit)
  (select (random 6) '(apple banana grape orange pear plum))) ; remember select?

;;; Variation 6: the random fruit basket of size n.
(define (basket n)
  (if (zero? n)
      '()
      (cons (fruit) (basket (sub1 n)))))

;;; We can do something similar with dice. The following function simulates a random roll
;;; of a single die that has n sides. It returns a random integer 1, 2, 3, ..., up to n.
(define (n-sided-die n)
  (add1 (random n)))

;;; Variation 7: roll n dice each having sides sides:
(define (roll-them-bones sides n)
  (if (zero? n)
      '()
      (cons (n-sided-die sides) (roll-them-bones sides (sub1 n)))))

;;; Try rolling two 20 sided dice by evaluating (roll-them-bones 20 2). If you get (20 20),
;;; go eat an ice-cream cone!

;;; Variation 8: Counting down

(define (count-down n)
  (if (zero? n)
      (list 0)
      (cons n (count-down (sub1 n)))))

;;; Verify that (count-down 10) -> (10 9 8 7 6 5 4 3 2 1 0).
;;; Can you modify count-down so that it ends with an explosive "BOOM!!!"?

;;; In order to count-up, we will need to reverse the order of a list. First we
;;; create post, as a helper function, to append an item to the end of a given list:

(define (post x s)
  (append s (list x)))

;;; Verify that (post "Curly" '("Larry" "Moe")) -> ("Larry" "Moe" "Curly").

;;; Variation 9: Reversing a list (also known as reverse):

(define (reversi s)
  (if (null? s)
      '()
      (post (car s) (reversi (cdr s)))))

;;; Verify that (reversi '(a b c d e)) -> (e d c b a). Racket has a built-in function called
;;; reverse that does exactly the same thing.

;;; Now we can use reversi to convert count-down to count-up:

(define (count-up n)
  (cdr (reversi (count-down n))))

;;; Verify that (count-up 10) -> (1 2 3 4 5 6 7 8 9 10). What happened to the 0 or "BOOM!!!"?

;;; Variation 10: Dropping the leading elements of a list.

(define (drop n s)
  (if (or (zero? n) (null? s))
      s
      (drop (sub1 n) (cdr s))))

;;; Verify that (drop 2 '(tic tac toe)) -> (toe)

;;; Variation 11: Taking only the leading elements of a list.

(define (take n s)
  (if (or (zero? n) (null? s))
      '()
      (cons (car s) (take (sub1 n) (cdr s)))))

;;; Verify that (take 2 '(tic tac toe)) -> (tic tac).

;;; Variation 12: Inserting something new into the i-th position of a list

(define (insert x i s)
  (if (zero? i)
      (cons x s)
      (cons (car s) (insert x (sub1 i) (cdr s)))))

;;; Verify that (insert 'gold 3 '(copper platinum lead aluminum)) -> (copper platinum lead gold aluminum).

;;; Variation 13: Deleting the i-th item from a list:

(define (delete i s)
  (if (zero? i)
      (cdr s)
      (cons (car s) (delete (sub1 i) (cdr s)))))

;;: Verify that (delete 2 '(all is not good)) -> (all is good).

;;; Variation 14: Swapping the i-th and j-th items in a list. Note that i should not exceed j,
;;; and neither should exceed the length of s.

(define (swap i j s)
  (if (zero? i)
      (cons (select j s) (insert (car s) (sub1 j) (cdr (delete j s))))
      (cons (car s) (swap (sub1 i) (sub1 j) (cdr s)))))

;;; What happens to (swap 0 2 '(I see RED)).

;;; Variation 15: Create a multiplication table.

(define (times-table m n)
  (if (zero? n)
      (list)
      (cons (map (lambda (x) (* (- m n -1) x)) (count-up m)) (times-table m (sub1 n)))))

;;; Try (times-table 12 12) or (times-table 20 20).


;;; Variation 16: iterating a function
(define (iterate f n x)
  (if (zero? n)
      x
      (f (iterate f (sub1 n) x))))

;;; Thus, (iterate f 0 x) -> x, (iterate f 1 x) -> (f x), (iterate f 2 x) -> (f (f x)),
;;; (iterate f 3 x) -> (f (f (f x))), etc. To test this out, evaluate
;;; (iterate add1 100 0), (iterate sub1 100 0). Or define,

(define (double x)
  (* 2 x))

;;; and evaluate (iterate double 1 10), or evaluate the following:
;;; (map (lambda (n) (iterate double n 1)) (reverse (count-down 1000)))
;;; Wow! Try computing the first thousand powers of 2 with pencil and paper.

;;; Variation 16: The greatest common divisor and least common multiple

;;; The greatest common divisor of two integers m and n is the greatest integer that divides
;;; evenly into each. For example (gcd 75 50) => 25. The following implements Euclid's
;;; algorithm. (Note: It's best to start with m >= n.)
(define (gcd m n)
  (if (zero? n)
      m
      (gcd n (modulo m n))))

;;; The gcd can be used to quickly compute the least common multiple of two integers m and n,
;;; that is, the smallest integer that both m and n divide into evenly. Thus,
;;; (lcm 8 6) => 24.
(define (lcm m n)
  (if (= m n 0)
      0
      (/ (* m n) (gcd m n))))

;;;;;;;;;;;


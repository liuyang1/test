#lang racket

;;; factorial.rkt
;;; Created by Robert R. Snapp, (C) 2011
;;;
;;; (N.B., ever character that follows a semicolon is ignored.)
;;;
;;; factorial computes n! = n*(n-1)*(n-2)* ... *3*2*1, where the "*" of course
;;; denotes mulitplication. The argument n should be a non-negative integer. If
;;; it is not, the function will run for a very long time, eventually triggering
;;; a stack overflow.

(define (factorial n)
  (if (= n 0)
      1
      (* n (factorial (- n 1)))))


;;; Although it is not required in this course, it is good programming
;;; style to defend against bad argument values that might trigger
;;; stack overflows.

;;; Here is a more safe version of factorial, which avoids the stack overflow bug.
;;; Note that it uses a function called error, which is defined in some
;;; of the available dialects, like "Pretty Big", but not in R5RS.
;;; Consequently, use language option "Pretty Big".

(define (factorial-safe n)
  (if (and (integer? n) (>= n 0))
      (factorial n)
      (error "Argument of factorial-safe is not a non-negative integer!")))

;;; Since factorial-safe is cumbersome to type, we can give it an abbreviation:

(define ! factorial-safe)

;;; It is also good programming practice to test each function as it is developed.
;;; Here we define a boolean parameter called *run-tests*. If it is set to #t,
;;; then a suite of tests is applied whenever this file is evaluated. Normally,
;;; one would place the following definition before the first function definition.
;;; But since this is the first time we mention it, we place it here to avoid
;;; confusion. Once you are certain everything works, you can then redefine
;;; *run-tests* to #f.

(define *run-tests* #t)

;;; Here is the test suite.

(if *run-tests*
    (begin
      (display "Testing function factorial-safe ... ")
      (if (and (= (factorial-safe 0) 1)
               (= (factorial-safe 1) 1)
               (= (factorial-safe 2) 2)
               (= (factorial-safe 3) 6)
               (= (factorial-safe 4) 24)
               (= (factorial-safe 5) 120)
               (= (factorial-safe 6) 720)
               (= (factorial-safe 7) 5040))
          (display "okay.")
          (error "function factorial-safe is broken!"))

      (newline)
      (display "Testing function ! ... ")
      (if (and (= (! 0) 1)
               (= (! 7) 5040))
          (display "okay.")
          (error "function ! is broken!")))
    (display "Skipping tests."))

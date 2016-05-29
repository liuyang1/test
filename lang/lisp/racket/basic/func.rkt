#lang racket
; default argument / optional arguement
(define (func [arg 42]) arg)

(func)
(func 7)

; keyword argument
(define greet
  (lambda (#:hi [hi "Hello"] given [surname "Smith"])
    (string-append hi ", " given " " surname)))

(greet "John")
(greet "John" "White")
(greet "John" #:hi "Morning")

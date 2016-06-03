#lang racket

(define (fn n)
  (if (<= n 0)
    (void)
    (begin (displayln "Hello World")
           (fn (- n 1)))))

(let ([a (read)])
 (fn a))

#lang racket
(define (eval-formula formula v)
  (eval `(let ([x ,v]
               [y 2])
           ,formula)
        (make-base-namespace)))

(eval-formula '(+ x y) 7)

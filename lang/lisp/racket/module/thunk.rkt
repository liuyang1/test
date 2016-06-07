#lang racket
(define thk (thunk (define x 1) (printf "~a\n" x)))

(thk)

(thk)

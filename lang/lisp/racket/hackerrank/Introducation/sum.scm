#lang racket

(define (sum-of-two-integers a b)
  (+ a b))

; Howto IO
(let ([a (read)]
      [b (read)])
  (printf "~a~%" (sum-of-two-integers a b)))

#lang racket

(define (ffib x)
  (let ((a (car x))
        (b (cdr x)))
    (cons b (+ a b))))

(define (iterate fn x0)
  (let ((x x0))))
(define (fib n)
  (map (lambda (x) (car x))
       (take n (iterate ffib (cons 1 1)))))

(fib 10)

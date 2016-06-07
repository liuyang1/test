#lang racket

(define (tail-add sym) (curryr list sym))

(map (tail-add 'foo) '(1 2 3))

(define (add-val x) (curryr + x))
(map (add-val 5) '(1 2 3))

;;; Curry with lambda function
(define pow
  (lambda (x)
    (lambda (y)
      (if (= y 0)
        1
        (* x ((pow x) (- y 1)))))))

(define (test-case pow-fn)
  (and
    (= ((pow 2) 4) 16)
    (= ((pow 3) 5) 243)))

(test-case pow)

;;; This is suger for defining a curried function at Racket.
(define ((pow-suger x) y)
  (if (= y 0)
    1
    (* x ((pow x) (- y 1)))))

(test-case pow-suger)

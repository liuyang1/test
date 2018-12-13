#lang racket

(define (product ls)
  (foldr * 1 ls))

(define (bingcd a b)
  (cond ((< a b) (bingcd b a))
        ((= b 0) a)
        ((= b 1) 1)
        ((even? a) (cond ((even? b) (* 2 (bingcd (/ a 2) (/ b 2))))
                         (else (bingcd (/ a 2) b))))
        (else (cond ((even? b) (bingcd a (/ b 2)))
                    (else (bingcd (/ (- a b) 2) b))))))

;;; IO part
; first read length of list, then read N integer, and build to a list
(define (read-len-lst)
  (define (iter n lst)
    (if (= n 0)
      lst
      (iter (- n 1) (cons (read) lst))))
  (iter (read) '()))

;;; test
; modulo 1000000007 for purpose
(let ([ls0 (read-len-lst)]
      [ls1 (read-len-lst)])
  (remainder (bingcd (product ls0) (product ls1))
             1000000007))

#lang racket

;;; implement function at this page: https://hackage.haskell.org/package/base-4.10.0.0/docs/Data-List.html

;;; Basic Functions
(define (++ ls0 ls1)
  (if (null? ls0)
    ls1
    (cons (car ls0) (++ (cdr ls0) ls1))))

(equal? (++ (list 1 2 3) (list 4 5 6))
        (range 1 7))

(define (head ls)
  (car ls))

(equal? (head (list 1 2 3))
        1)

(define (tail ls)
  (cdr ls))

(equal? (tail (list 1 2 3))
        (list 2 3))

(define (init ls)
  (cond ((null? ls) (error "init: must be non-empty"))
        ((null? (cdr ls)) '())
        (else (cons (car ls) (init (cdr ls))))))

(and (equal? (init (list 1))
             '())
     (equal? (init (list 1 2))
             '(1))
     (equal? (init (list 1 2 3))
             '(1 2)))

(define (last ls)
  (cond ((null? ls) (error "last: must be non-empty"))
        ((null? (cdr ls)) (car ls))
        (else (last (cdr ls)))))

(and (equal? (last (list 1))
             1)
     (equal? (last (list 1 2))
             2)
     (equal? (last (list 1 2 3))
             3))

;;; TODO:
;;; uncons
;;; null

(define (length1 ls)
  (if (null? ls)
    0
    (+ 1 (length1 (cdr ls)))))

(equal? (length1 (list 1 2 3))
        3)

;;; List Transfroms

(define (map1 f ls)
  (if (null? ls)
    '()
    (cons (f (car ls)) (map1 f (cdr ls)))))

(equal? (map1 (lambda (x) (+ 1 x)) (range 5))
        (range 1 6))


;;; REMIND: How to write this function with recurisivly ways???
(define (reverse1 ls)
  (define (helper xs ys)
    (if (null? xs)
      ys
      (helper (cdr xs) (cons (car xs) ys))))
  (helper ls '()))

(equal? (reverse1 (range 3))
        '(2 1 0))

(define (intersperse x xs)
  (if (or (null? xs) (null? (cdr xs)))
    xs
    (cons (car xs)
          (cons x
                (intersperse x (cdr xs))))))

(and
  (equal? (intersperse -1 (range 3))
          '(0 -1 1 -1 2))
  (equal? (intersperse -1 (range 2))
          '(0 -1 1))
  (equal? (intersperse -1 (range 1))
          '(0))
  (equal? (intersperse -1 (range 0))
          '()))

;;; TOOD: after concat
; (define (intercalate xs xss))

; (define (transpose xss))

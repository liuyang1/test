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

;;; Reducing Lists (folds)

;; foldl :: Foldable t => (b -> a -> b) -> b -> t a -> b
(define (foldr op unit xs)
  (if (null? xs)
    unit
    (op (car xs)
        (foldr op unit (cdr xs)))))

(equal? (foldr cons '() (range 3))
        '(0 1 2))

(define (foldl op unit xs)
  (if (null? xs)
    unit
    (foldl op (op unit (car xs)) (cdr xs))))

(equal? (foldl cons '() (range 3))
        '(((() . 0) . 1) . 2))

(define (foldr1 op xs)
  (if (null? (cdr xs))
    (car xs)
    (op (car xs)
        (foldr1 op (cdr xs)))))

(equal? (foldr1 / (range 1 5))
        (/ 1 (/ 2 (/ 3 4))))

;; fold1 :: Foldable t => (a -> a -> a) -> t a -> a
(define (foldl1 op xs)
  (if (null? (cdr xs))
    (car xs)
    (foldl op (car xs) (cdr xs))))

(equal? (foldl1 / (range 1 5))
        (/ (/ (/ 1 2) 3) 4))

(define (concat xss)
  (foldr ++ '() xss))

(define (transpose xss)
  (let ((new (filter (lambda (xs) (not (null? xs))) xss)))
   (if (null? new)
     '()
     (cons (map car new)
           (transpose (map cdr new))))))

(and (equal? (transpose '((10 11) (20) () (30 31 32)))
             '((10 20 30) (11 31) (32)))
     (equal? (transpose '((10 11 12) (20 21 22) (30 31 32)))
             '((10 20 30) (11 21 31) (12 22 32))))

;; subsequences:: [a] -> [[a]]
(define (subsequences xs)
  (if (null? xs)
    '(())
    (let ((x (car xs))
          (ys (subsequences (cdr xs))))
      (++ (map (lambda (a) (cons x a)) ys)
          ys))))

(and (equal? (subsequences '())
             '(()))
     (equal? (subsequences '(1))
             '((1) ()))
     (equal? (subsequences '(1 2))
             '((1 2) (1) (2) ()))
     (equal? (subsequences '(1 2 3))
             '((1 2 3) (1 2) (1 3) (1) (2 3) (2) (3) ())))

(define (insert x idx xs)
  (if (= idx 0)
    (cons x xs)
    (cons (car xs) (insert x (- idx 1) (cdr xs)))))

;; insert-every:: a -> [a] -> [[a]]
(define (insert-every x xs)
  (map (lambda (idx) (insert x idx xs))
       (range 0 (+ 1 (length xs)))))

(equal? (insert-every -1 (range 0 3))
        '((-1 0 1 2) (0 -1 1 2) (0 1 -1 2) (0 1 2 -1)))

;; permutations:: [a] -> [[a]]
(define (permutations xs)
  (if (or (null? xs) (null? (cdr xs)))
    (list xs)
    (let ((x (car xs))
          (ys (permutations (cdr xs))))
      (concat
        (map (lambda (y) (insert-every x y))
             ys)))))

(and (equal? (permutations '())
             '(()))
     (equal? (permutations '(1))
             '((1)))
     (equal? (permutations '(1 2))
             '((1 2) (2 1)))
     (equal? (permutations '(1 2 3))
             '((1 2 3) (2 1 3) (2 3 1) (1 3 2) (3 1 2) (3 2 1))))

;;; Special folds

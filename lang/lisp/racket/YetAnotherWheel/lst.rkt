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

;; foldl1 :: Foldable t => (a -> a -> a) -> t a -> a
(define (foldl1 op xs)
  (if (null? (cdr xs))
    (car xs)
    (foldl op (car xs) (cdr xs))))

(equal? (foldl1 / (range 1 5))
        (/ (/ (/ 1 2) 3) 4))

(define (concat xss)
  (foldr ++ '() xss))

(define (intercalate xs xss)
  (concat (intersperse xs xss)))

(equal? (intercalate '(1 1) '((0) (0) (0)))
        '(0 1 1 0 1 1 0))

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
(define (concatMap f xs)
  (concat (map f xs)))

(equal? (concatMap (lambda (x) (range (+ 1 x)))
                   (range 5))
        '(0 0 1 0 1 2 0 1 2 3 0 1 2 3 4))

(define (and1 xs)
  (define (and2 x y) (and x y))
  (foldl and2 #t xs))

(and
  (equal? (and1 '())
          #t)
  (equal? (and1 '(#t #t #t))
          #t)
  (equal? (and1 '(#t #t #f))
          #f))

(define (or1 xs)
  (define (or2 x y) (or x y))
  (if (null? xs) #t
    (foldl or2 #f xs)))

(and
  (equal? (or1 '())
          #t)
  (equal? (or1 '(#f #f #f))
          #f)
  (equal? (or1 '(#t #t #f))
          #t))

(define (any f xs)
  (or1 (map f xs)))

(equal? (any (lambda (x) (= (remainder x 2) 0))
             (range 3))
        #t)

(define (all f xs)
  (and1 (map f xs)))

(equal? (all (lambda (x) (= (remainder x 2) 0))
             (range 3))
        #f)

(define (sum xs)
  (foldl + 0 xs))

(equal? (sum (range 11))
        55)

(define (product xs)
  (foldl * 1 xs))

(equal? (product (range 1 6))
        120)

(define (maximum xs)
  (foldl1 max xs))

(equal? (maximum (range 10))
        9)

(define (minimum xs)
  (foldl1 min xs))

(equal? (minimum (range 10))
        0)
;;; Building Lists
;;; Scans

(define (scanl op unit xs)
  (if (null? xs)
    (list unit)
    (let ((v (op unit (car xs))))
     (cons unit (scanl op v (cdr xs))))))

(equal? (scanl * 1 (range 1 6))
        '(1 1 2 6 24 120))

(define (scanl1 op xs)
  (if (or (null? xs) (null? (cdr xs)))
    xs
    (scanl op (car xs) (cdr xs))))

(and (equal? (scanl1 * (range 1 6))
             '(1 2 6 24 120))
     (equal? (scanl1 * '(2))
             '(2)))

(define (scanr op unit xs)
  (if (null? xs)
    (list unit)
    (let ((v (scanr op unit (cdr xs))))
     (cons (op (car xs) (car v))
           v))))

(equal? (scanr * 1 (range 1 6))
        '(120 120 60 20 5 1))

(define (scanr1 op xs)
  (if (or (null? xs) (null? (cdr xs)))
    xs
    (scanr op (last xs) (init xs))))

(equal? (scanr1 * (range 1 6))
        '(120 120 60 20 5))

;;; Infinite lists
;;; Just implement with times here

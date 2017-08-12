#lang racket

;;; implement function at this page: https://hackage.haskell.org/package/base-4.10.0.0/docs/Data-List.html

;;; Basic Functions
;;; same function with LISP's append
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

;; combinations :: Z -> [a] -> [[a]]
(define (combinations n xs)
  (cond ((or (= n 0) (null? xs)) '())
        ((= n 1) (map list xs))
        (else (++ (map (lambda (x) (cons (car xs) x))
                       (combinations (- n 1) (cdr xs)))
                  (combinations n (cdr xs))))))

(and (equal? (combinations 3 (range 5))
             '((0 1 2) (0 1 3) (0 1 4) (0 2 3) (0 2 4) (0 3 4) (1 2 3) (1 2 4) (1 3 4) (2 3 4)))
     (equal? (combinations 3 (range 2))
             '()))

;;; Special folds
(define (concatMap f xs)
  (concat (map f xs)))

(equal? (concatMap (lambda (x) (range (+ 1 x)))
                   (range 5))
        '(0 0 1 0 1 2 0 1 2 3 0 1 2 3 4))

;;; combine :: [a] -> [b] -> [(a, b)]
(define (combine xs ys)
  (concatMap (lambda (x)
               (map (lambda (a) (cons x a)) ys))
             xs))

(equal? (combine (range 10 30 10) (range 4))
        '((10 . 0) (10 . 1) (10 . 2) (10 . 3) (20 . 0) (20 . 1) (20 . 2) (20 . 3)))

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

(define (iterate f a times)
  (if (= times 0)
    '()
    (cons a (iterate f (f a) (- times 1)))))

(equal? (iterate (lambda (x) (+ 1 x)) 0 4)
        '(0 1 2 3))

(define (repeat a times)
  (if (= times 0)
    '()
    (cons a (repeat a (- times 1)))))

(equal? (repeat 0 5)
        '(0 0 0 0 0))

(define (cycle xs times)
  (if (= times 0)
    '()
    (++ xs (cycle xs (- times 1)))))

(equal?
  (cycle '(1 2 3) 3)
  '(1 2 3 1 2 3 1 2 3))


;;; Sublists
(define (take n xs)
  (if (= 0 n)
    '()
    (cons (car xs) (take (- n 1) (cdr xs)))))

(equal? (take 3 (range 10))
        '(0 1 2))

(define (drop n xs)
  (if (= 0 n)
    xs
    (drop (- n 1) (cdr xs))))

(equal?
  (drop 3 (range 10))
  '(3 4 5 6 7 8 9))

(define (splitAt n xs)
  (cons (take n xs)
        (drop n xs)))

(equal?
  (splitAt 3 (range 10))
  '((0 1 2) 3 4 5 6 7 8 9))

(define (takeWhile f xs)
  (if (not (f (car xs)))
    '()
    (cons (car xs) (takeWhile f (cdr xs)))))

(equal?
  (takeWhile (lambda (x) (< x 3)) '(1 2 3 4 1 2 3 4))
  '(1 2))

(define (dropWhile f xs)
  (if (f (car xs))
    (dropWhile f (cdr xs))
    xs))

(equal?
  (dropWhile (lambda (x) (< x 3)) '(1 2 3 4 5 1 2 3))
  '(3 4 5 1 2 3))

(define (span p xs)
  (cons (takeWhile p xs)
        (dropWhile p xs)))

(define (break p xs)
  (define (p1 x) (not (p x)))
  (span p1 xs))

(and
  (equal? (span (lambda (x) (< x 3)) (range 10))
          '((0 1 2) 3 4 5 6 7 8 9))
  (equal? (break (lambda (x) (>= x 3)) (range 10))
          '((0 1 2) 3 4 5 6 7 8 9)))

(define (stripPrefix xs ys)
  (cond ((null? xs) ys)
        ((= (car xs) (car ys)) (stripPrefix (cdr xs) (cdr ys)))
        (else '())))

(and
  (equal? (stripPrefix (range 3) (range 10))
          '(3 4 5 6 7 8 9))
  (equal? (stripPrefix '(1) (range 10))
          '()))

(define (group xs)
  (define (helper xs st)
    (if (null? xs)
      (list st)
      (let ((x (car xs)))
       (if (= x (car st))
         (helper (cdr xs) (cons x st))
         (cons st (helper (cdr xs) (list x)))))))
  (if (null? xs)
    '()
    (helper (cdr xs) (list (car xs)))))

(and (equal? (group '(0 1 1 2 2 3 3 4 5 5))
             '((0) (1 1) (2 2) (3 3) (4) (5 5)))
     (equal? (group '(1 1))
             '((1 1)))
     (equal? (group '(1))
             '((1)))
     (equal? (group '())
             '()))

;;; inits :: [a] -> [[a]]
;;; TODO: need pass () as first value
(define (inits xs)
  (if (null? xs)
    '()
    (let ((x (car xs)))
     (map (lambda (a) (cons x a))
          (cons '() (inits (cdr xs)))))))

(and (equal? (inits '())
             '())
     (equal? (inits (range 1))
             '((0)))
     (equal? (inits (range 2))
             '((0) (0 1)))
     (equal? (inits (range 3))
             '((0) (0 1) (0 1 2)))
     (equal? (inits (range 4))
             '((0) (0 1) (0 1 2) (0 1 2 3))))

;;; :) funny, I like it.
(define (tails xs)
  (reverse (map reverse (inits (reverse xs)))))

(equal? (tails (range 4))
        '((0 1 2 3) (1 2 3) (2 3) (3)))

;;; Predicates

(define (is-prefix-of xs ys)
  (cond ((null? xs) #t)
        ((null? ys) #f)
        ((= (car xs) (car ys)) (is-prefix-of (cdr xs) (cdr ys)))
        (else #f)))

(and (equal? (is-prefix-of '(3 1 4) '(3 1 4 1 5))
             #t)
     (equal? (is-prefix-of '(3 5 1) '(3 1 4 1 5))
             #f))

(define (is-suffix-of xs ys)
  (is-prefix-of (reverse1 xs)
                (reverse1 ys)))

(and (equal? (is-suffix-of '(1 5) '(3 1 4 1 5))
             #t)
     (equal? (is-suffix-of '(5 1) '(3 1 4 1 5))
             #f))

(define (is-subsequences xs ys)
  (cond ((null? xs) #t)
        ((null? ys) #f)
        ((= (car xs) (car ys)) (is-subsequences (cdr xs) (cdr ys)))
        (else (is-subsequences xs (cdr ys)))))

(and (equal? (is-subsequences '(1 1) '(3 1 4 1 5))
             #t)
     (equal? (is-subsequences '(1 4 5 1) '(3 1 4 1 5))
             #f))

(define (is-infix-of xs ys)
  (define (helper x y st)
    (cond ((null? x) st)
          ((null? y) #f)
          ((= (car x) (car y)) (helper (cdr x) (cdr y) #t))
          (else (helper xs (cdr y) #f))))
  (helper xs ys #f))

(and (equal? (is-infix-of '(1 4) '(3 1 4 1 5))
             #t)
     (equal? (is-infix-of '(1 1) '(3 1 4 1 5))
             #f))

;;; Searching Lists
;;; Searching by equality

(define (elem x xs)
  (cond ((null? xs) #f)
        ((= x (car xs)) #t)
        (else (elem x (cdr xs)))))

(and (equal? (elem 3 (range 0))
             #f)
     (equal? (elem 3 (range 5))
             #t)
     (equal? (elem 10 (range 5))
             #f))

(define (notElem x xs)
  (not (elem x xs)))

(define (find1 p xs)
  (if (null? xs)
    '()
    (let ((x (car xs)))
     (if (p x)
       x
       (find1 p (cdr xs))))))

(equal? (find1 (lambda (x) (= (remainder x 5) 0)) (range 1 20))
        5)

(define (filter1 p xs)
  (if (null? xs)
    '()
    (let ((x (car xs)))
     (if (p x)
       (cons x (filter1 p (cdr xs)))
       (filter1 p (cdr xs))))))


(equal? (filter1 (lambda (x) (= (remainder x 5) 0)) (range 1 20))
        '(5 10 15))

(define (partition1 p xs)
  (define (helper xs pos neg)
    (if (null? xs)
      (cons (reverse1 pos) (reverse1 neg))
      (let ((x (car xs)))
       (if (p x)
         (helper (cdr xs) (cons x pos) neg)
         (helper (cdr xs) pos (cons x neg))))))
  (helper xs '() '()))

(equal? (partition1 (lambda (x) (= (remainder x 5) 0)) (range 10))
        '((0 5) 1 2 3 4 6 7 8 9))

;;; Indexing lists

(define (!! xs i)
  (if (= i 0)
    (car xs)
    (!! (cdr xs) (- i 1))))

(equal? (!! (range 5) 3)
        3)

(define (findIndex f xs)
  (define (helper f xs i)
    (cond ((null? xs) -1)
          ((f (car xs)) i)
          (else (helper f (cdr xs) (+ i 1)))))
  (helper f xs 0))

(define (elemIndex x xs)
  (findIndex (lambda (a) (= a x)) xs))

(equal? (elemIndex 3 (range 5))
        3)

(define (findIndices f xs)
  (define (helper f xs i)
    (cond ((null? xs) '())
          ((f (car xs)) (cons i (helper f (cdr xs) (+ i 1))))
          (else (helper f (cdr xs) (+ i 1)))))
  (helper f xs 0))

(define (elemIndices x xs)
  (findIndices (lambda (a) (= a x)) xs))

(equal? (elemIndices 1 '(0 0 1 0 1 1 0))
        '(2 4 5))

;;; Zipping and Unzipping lists

(define (zip xs ys)
  (if (or (null? xs) (null? ys))
    '()
    (cons (list (car xs) (car ys))
          (zip (cdr xs) (cdr ys)))))

(equal? (zip (range 10) (range 10 15))
        '((0 10) (1 11) (2 12) (3 13) (4 14)))

(define (zip3 xs ys zs)
  (if (or (null? xs) (null? ys) (null? zs))
    '()
    (cons (list (car xs) (car ys) (car zs))
          (zip3 (cdr xs) (cdr ys) (cdr zs)))))

(equal? (zip3 (range 10 ) (range 10 15) (range 20 22))
        '((0 10 20) (1 11 21)))

(define (zips xss)
  (if (or1 (map null? xss))
    '()
    (cons (map car xss)
          (zips (map cdr xss)))))

(equal? (zips (list (range 10 ) (range 10 15) (range 20 22)))
        '((0 10 20) (1 11 21)))

(define (zipWith f xs ys)
  (if (or (null? xs) (null? ys))
    '()
    (cons (f (car xs) (car ys))
          (zipWith f (cdr xs) (cdr ys)))))

(equal? (zipWith + (range 10) (range 0 40 10))
        '(0 11 22 33))

;;; "Set" operations

(define (nub xs)
  (if (null? xs)
    '()
    (let ((x (car xs)))
     (cons x
           (nub (filter (lambda (a) (not (= a x))) (cdr xs)))))))

(equal? (nub '(0 0 1 1 3 4 1 0 3))
        '(0 1 3 4))

(define (deleteBy p xs)
  (cond ((null? xs) '())
        ((p (car xs)) (cdr xs))
        (else (cons (car xs) (deleteBy p (cdr xs))))))

(define (delete x xs)
  (deleteBy (lambda (a) (= a x)) xs))

(equal? (delete 1 '(3 1 4 1 5 9 2 6))
        '(3 4 1 5 9 2 6))

;;; Ordered lists

; quick sort
; LISP is so easy!
(define (sort xs)
  (if (or (null? xs) (null? (cdr xs)))
    xs
    (letrec ((x (car xs))
             (ys (partition1 (lambda (a) (< a x)) (cdr xs))))
      (++ (sort (car ys))
          (cons x
                (sort (cdr ys)))))))

(equal? (sort '(3 1 4 1 5 9 2 6 5 4 5 8 9))
        '(1 1 2 3 4 4 5 5 5 6 8 9 9))

(define (sortOn f xs)
  (if (or (null? xs) (null? (cdr xs)))
    xs
    (letrec ((x (car xs))
             (ys (partition1 (lambda (a) (< (f a) (f x))) (cdr xs))))
      (++ (sortOn f (car ys))
          (cons x
                (sortOn f (cdr ys)))))))

(equal? (sortOn (lambda (x) (- x)) '(3 1 4 1 5 9 2 6 5 4 5 8 9))
        (reverse1 '(1 1 2 3 4 4 5 5 5 6 8 9 9)))

;;; splits :: [a] -> [([a], [a])]
(define (splits xs)
  (let ((nil (list '())))
   (zip (++ nil (inits xs)) (++ (tails xs) nil))))

(equal? (splits (range 3))
        '((() (0 1 2)) ((0) (1 2)) ((0 1) (2)) ((0 1 2) ())))

(define (factorial n)
  (if (or (= n 0) (= n 1))
    1
    (* n (factorial (- n 1)))))
(define (permutation-num n k)
  (product (range (- n k -1) (+ n 1))))

(and (equal? (factorial 6)
             720)
     (equal? (permutation-num 6 3)
             120))

;;; (a1 + a2 + a3 + ...)!
;;; ----------------------
;;; a1! * a2! * a3! * ...
(define (permutation-group-num x xs)
  (/ (factorial x)
     (product (map factorial xs))))

(and (equal? (permutation-group-num 5 '(3 2))
             10)
     (equal? (permutation-group-num 5 '(2 2))
             30))

;;; permutation-group :: [[a]] -> [[a]]
(define (permutation-group xss)
  (define (helper xs g)
    (if (null? xs)
      (list g)
      (let ((ss (splits g))
            (x (car xs)))
        (concatMap (lambda (s)
                     (let ((b (car s))
                           (e (cadr s)))
                       (let ((r (helper (cdr xs) e)))
                        (map (lambda (ne) (++ b (cons x ne)))
                             r))))
                   ss))))
  (define (f xs gs)
    (concatMap (lambda (g) (helper xs g))
               gs))
  (if (or (null? xss) (null? (cdr xss)))
    xss
    (foldr f '(()) xss)))

(and
  (equal? (permutation-group '((2 2)))
          '((2 2)))
  (equal? (permutation-group '((1) (2 2)))
          '((1 2 2) (2 1 2) (2 2 1)))
  (equal? (permutation-group '((1 1 1) (2 2)))
          '((1 1 1 2 2) (1 1 2 1 2) (1 1 2 2 1) (1 2 1 1 2) (1 2 1 2 1) (1 2 2 1 1) (2 1 1 1 2) (2 1 1 2 1) (2 1 2 1 1) (2 2 1 1 1)))
  (equal? (permutation-group '((4 4) (1) (2 2)))
          '((4 4 1 2 2) (4 1 4 2 2) (4 1 2 4 2) (4 1 2 2 4) (1 4 4 2 2) (1 4 2 4 2) (1 4 2 2 4) (1 2 4 4 2) (1 2 4 2 4) (1 2 2 4 4) (4 4 2 1 2) (4 2 4 1 2) (4 2 1 4 2) (4 2 1 2 4) (2 4 4 1 2) (2 4 1 4 2) (2 4 1 2 4) (2 1 4 4 2) (2 1 4 2 4) (2 1 2 4 4) (4 4 2 2 1) (4 2 4 2 1) (4 2 2 4 1) (4 2 2 1 4) (2 4 4 2 1) (2 4 2 4 1) (2 4 2 1 4) (2 2 4 4 1) (2 2 4 1 4) (2 2 1 4 4)))
  (equal? (length (permutation-group '((4 4) (1) (2 2))))
          (permutation-group-num 5 '(2 1 2))))

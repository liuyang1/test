#lang racket

;; count-down generates a reverse numerical sequence from n to 0. (count-down 3) => '(3 2 1 0)
(define (count-down n)
  (if (< n 0)
      '()
      (cons n (count-down (sub1 n)))))
;; count-up generates a numerical sequence in increasing order from 0 to n. (count-up 3) => '(0 1 2 3)
(define (count-up n)
  (reverse (count-down n)))

;; select n s selects the n-th element is list s.
(define (select n s)
  (if (zero? n)
      (car s)
      (select (sub1 n) (cdr s))))

;;; Let's learn how to shuffle a deck of cards. First we define the deck:

(define deck (count-up 51))

(define (rank card)
  (select (remainder card 13) '(A 2 3 4 5 6 7 8 9 10 J Q K)))

(define (suit card)
  (select (floor (/ card 13)) '(C D H S)))

(define (card-name card)
  (list (rank card) (suit card)))

;;; Evaluate deck. Now we need to cut it.
(define (cut s)
  (let ((n (length s)))
    (list (take s (floor (/ n 2))) (drop s (- n (floor (/ n 2)))))))

;;; Or better yet, we can define split to split a list into n sublists:

(define (chew s n)
  (if (<= (length s) n)
      (list s)
      (cons (take n s) (chew (drop n s) n))))

(define (split s n)
  (let ((bite-size (max 1 (/ (count s) n))))
    (chew s bite-size)))

(define (riffle p q)
  (cond ((null? p) q)
        ((null? q) p)
        (else (cons (car p) (cons (car q) (riffle (cdr p) (cdr q)))))))

(define (random-riffle p q)
  (cond ((null? p) q)
        ((null? q) p)
        ((zero? (random 2)) (cons (car p) (cons (car q) (random-riffle (cdr p) (cdr q)))))
        (else (cons (car q) (cons (car p) (random-riffle (cdr p) (cdr q)))))))

(define (random-shuffle s)
  (random-riffle (car (cut s)) (cadr (cut s))))

;;; Dealers and magicians would call the following an
(define (out-shuffle s)
  (riffle (car (cut s)) (cadr (cut s))))

(define (in-shuffle s)
  (riffle (cadr (cut s)) (car (cut s))))

;;; Try it. What does (out-shuffle (out-shuffle (out-shuffle (out-shuffle (out-shuffle (out-shuffle (out-shuffle (out-shuffle deck))))))))
;;; evaluate to?


;;; Evaluate (iterate out-shuffle deck 1), (iterate out-shuffle deck 2), ... (iterate out-shuffle deck 8), or rather
;;; (map (lambda (x) (iterate out-shuffle deck x)) (count-up 8))

;;; Also evaluate:
;;; (map (lambda (x) (iterate in-shuffle deck x)) (count-up 52))

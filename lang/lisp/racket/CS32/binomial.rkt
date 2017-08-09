#lang racket

;;; binomial.rkt
;;;
;;; Copyright 2006, Robert R. Snapp,


;;; (binom r k) returns the binomial coefficient, or "r choose k," generalized to real
;;; (or complex-valued) r and integer k. If r is an integer, this number corresponds to
;;; the number of ways that k objects can be selected from a set of size r. Explicitly,
;;; this function evaluates the expression:
;;;
;;;       r!       r (r - 1) (r - 2) ... (r - k + 1)
;;;  ----------- = ----------------------------------
;;;  k! (r - k)!        k (k - 1) (k - 2) ... 1
;;;
;;;                 r - k + 1     r - (k - 1) + 1           r - 2 + 1     r - 1 + 1
;;;              = ----------- * ----------------- * ... * ----------- * -----------
;;;                     k              k - 1                    2             1
;;;
;;; This function requires k - 1 multiplications and k divisions.

(define (binom r k)
  (cond ((not (integer? k)) (error "binom: second argument must be an integer."))
        ((negative? k) 0)
        (else (letrec ([aux (lambda (value i)
                              (cond ((zero? i) value)
                                    (else (aux (* value (/ (- r i -1) i)) (- i 1)))))])
                (aux 1 k)))))

; >>> The author like to use `letrec` to define a local function
(define (binom1 r k)
  (define (aux v i)
    (if (= i 0)
      v
      (aux (* v
              (/ (- r i -1) i))
           (- i 1))))
  (cond ((not (integer? k)) (error "binom: second argument must be an integer."))
        ((negative? k) 0)
        (else (aux 1 k))))

;; Function (binom-test) returns #t if all tests of function (binom r k) are successful.
(define (binom-test)
  (let ((nTests  0)
        (nPassed 0))
    (letrec ((test (lambda (expr str target)
                     (set! nTests (+ nTests 1))
                     (if (= expr target)
                       (set! nPassed (+ nPassed 1))
                       (printf "  Error: ~a = ~a != target ~a~n" str expr target)))))
      (begin
        (display "Testing (binom r k):\n")
        (test (binom  0  0)  "(binom  0  0)"  1)
        (test (binom  5  5)  "(binom  5  5)"  1)
        (test (binom  4  2)  "(binom  4  2)"  6)
        (test (binom  4  5)  "(binom  4  5)"  0)
        (test (binom  4 -4)  "(binom  4 -4)"  0)
        (test (binom -3  2)  "(binom -3  2)"  6)

        (printf "  SUMMARY: passed tests = ~a, failed tests = ~a~n"
                nPassed (- nTests nPassed))))
    (= nTests nPassed)))

(binom-test)


;;; (binomial n k) also evaluates the binomial coefficent,
;;;
;;;      n!
;;;  ------------,
;;;  k! (n - k )!
;;;
;;; for integer n and k, but by a different method. Here, we compute the corresponding value in Pascal's triangle
;;; using recursion and mainly addition. For example,
;;;
;;;   (binomial 0 0) =>  1
;;;   (binomial 9 3) => 84
;;;
;;; Note that the binomial is defined for arbitrary integers n and k. If n is negative, we apply
;;; the identity,
;;;
;;;      n!                 (k - n - 1)!
;;;  ----------- = (-1)^k * ------------
;;;  k! (n - k)!            k! (-n - 1)!

(define (binomial n k)
  (letrec ((binomial-pos (lambda (m i)
                           (car (pascal m i i)))))
    (cond ((not (integer? k)) (error "binomial : second argument must be an integer."))
          ((negative? k) 0)
          ((negative? n) (* (expt -1 k) (binomial-pos (- k n 1) k)))
          (else (binomial-pos n k)))))

;;; (pascal d first-col last-col) returns a list containing partial row of the integers in the
;;; d-th row of Pascal's triangle. Here, d denotes the depth of the row. first-col and last-col
;;; denote the column indices of the first and last values that will be printed. Thus,
;;; (pascal d 0 d) generates the entire d-th row, and (pascal d c c) returns a singleton list.
;;; For example,
;;;
;;;    (pascal 3 0 3) => (1 3 3 1)
;;;    (pascal 4 1 2) => (4 6)
;;;    (pascal 6 3 3) => (20)

(define (pascal d first-col last-col)
  (cond ((zero? d) '(1))
        (else (letrec ((aux (lambda (buffer count prev)
                              (printf "~a , ~a , ~a~n" buffer count prev)
                              (cond ((or (null? prev) (zero? count)) (reverse buffer))
                                    ((and (zero? first-col) (null? buffer)) (aux '(1) (- count 1) prev))
                                    (else (let ((value (if (null? (cdr prev))
                                                         (car prev)
                                                         (+ (car prev) (cadr prev)))))
                                            (aux (cons value buffer) (- count 1) (cdr prev)))))))
                       (d-prev (- d 1))
                       (first-prev (max 0 (- first-col 1)))
                       (last-prev  (min last-col d-prev))
                       (n-cols (+ (- last-col first-col) 1)))
                (aux '() n-cols (pascal d-prev first-prev last-prev))))))

(pascal 6 3 3)

;;; Additionally, we can define the multinomial coefficient. Thus (multinomial n k1 k2 ... km)
;;; returns the expression
;;;
;;;                 n!
;;;  ---------------------------------------
;;;  k1! k2! ... km! (n - k1 - k2 - ... km)!
;;;
;;; which is equal to the product of the binomials:
;;;
;;;       n!             (n - k1)!               (n - k1 - ... - k[m-1])!
;;;  ------------- * ------------------ * ... * ----------------------------
;;;  k1! (n - k1)!   k2! (n - k1 - k2)!          km! * (n - k1 - ... - km)!
;;;
;;;
;;; Thus,
;;; (multinomial 11 4 4 2) => 34650, the number of unique anagrams of the letters in "MISSISSIPPI"
;;; >>> I 4, S 4, P 2, M 1 => (multinomial (+ 4 4 2 1) 4 4 2)
;;; (Note that (multinomial 11 4 4 2 1) returns the same value, but (multinomial 11 4 4 2 1 1) returns 0.

(define multinomial
  (lambda (n . k-list)
    (cond ((null? k-list) (error "Syntax: (multinomial n k1 ... km)"))
          ((negative? n) (error "multinomial: n should be non-negative"))
          ((member #t (map negative? k-list)) (error "multinomial: k1, ..., km should be non-negative."))
          (else (letrec ((aux (lambda (value n l)
                                (cond ((null? l) value)
                                      ((< n (car l)) 0)
                                      (else (aux (* value (binom n (car l)))
                                                 (- n (car l))
                                                 (cdr l)))))))
                  (aux 1 n k-list))))))

#lang racket
;;; anteater.scm
;;; Created by Robert R. Snapp, (C) 2011
;;;
;;; Here again, is our boolean test parameter, which you can set to #f after your
;;; work is stable and working.

(define *run-tests* #t)

;;; Function anteater eats all of the ants in a "shallow" list. For example,
;;; (anteater '(ant toad worm ant)) => (toad worm).

;;; Since we are assuming a "shallow" list, one that does not contain other
;;; lists as elements, we will use the pattern provided by clone:

(define (anteater s)                                    ; Essentially line 1 of clone.
  (cond ((null? s) '())                                 ; Exactly line 2 of clone
        ((eq? (car s) 'ant) (anteater (cdr s)))         ; New functionality!
        (else (cons (car s) (anteater (cdr s))))))      ; Essentially line 3 of clone.

;;; Here we test anteater.

(and *run-tests*
    (begin
      (display "Testing anteater ... ")
      (if (and (equal? (anteater '()) '())
               (equal? (anteater '(ant)) '())
               (equal? (anteater '(cow)) '(cow))
               (equal? (anteater '(ant toad worm ant)) '(toad worm))
               (equal? (anteater '(cow (ant) bee)) '(cow (ant) bee)))
          (displayln "okay.")
          (error "function anteater is broken!"))))

;;; Note in the last test (anteater '(cow (ant) bee)) => (cow (ant) bee). That is
;;; anteater can only eat ants that appear at the top level in a list. However, we
;;; can vary the pattern from deep-clone to create a more general version of anteater,
;;; that eats all ants. So using the definition below,
;;;
;;;    (deep-anteater '(cow (ant) bee)) => (cow () bee), and,
;;;    (deep-anteater 'ant) returns nothing!

(define (deep-anteater s)
  (if (pair? s)
      (if (eq? (car s) 'ant)
          (deep-anteater (cdr s))
          (cons (deep-anteater (car s)) (deep-anteater (cdr s))))
      (if (not (eq? s 'ant))
          s
          ; returns nothing if s mathches 'ant
          '())))


;;; And here we test it

(and *run-tests*
    (begin
      (display "Testing deep-anteater ... ")
      (if (and (equal? (deep-anteater '()) '())
               (equal? (deep-anteater '(ant)) '())
               (equal? (deep-anteater '(cow)) '(cow))
               (equal? (deep-anteater '(ant toad worm ant)) '(toad worm))
               (equal? (deep-anteater '(cow (ant) bee)) '(cow () bee))
               (equal? (deep-anteater 0) 0)
               (equal? (deep-anteater #\a) #\a)
               (equal? (deep-anteater "hello") "hello")
               (equal? (deep-anteater '(ant (ant (ant) ant) ant)) '((()))))
          (displayln "okay.")
          (error "function anteater is broken!"))))

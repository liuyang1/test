#lang racket
;;; alchemy.rkt
;;; Created by Robert R. Snapp, (C) 2011
;;;
;;; Here again, is our boolean test parameter, which you can set to #f after your
;;; work is stable and working.

(define *run-tests* #t)

;;; Function alchemy turns every occurance of 'lead in a "shallow" list into gold.
;;; (alchemy '(tin gold wood lead silver lead)) => (tin gold wood gold silver gold).

;;; Since we are assuming a "shallow" list, one that does not contain other
;;; lists as elements, we will use the pattern provided by clone:

(define (alchemy s)                                             ; Essentially line 1 of clone.
  (cond ((null? s) '())                                         ; Exactly line 2 of clone
        ((eq? (car s) 'lead) (cons 'gold (alchemy (cdr s))))    ; New functionality (but similar to anteater)
        (else (cons (car s) (alchemy (cdr s))))))               ; Essentially line 3 of clone.

;;; Here we test alchemy.

(if *run-tests*
    (begin
      (display "Testing alchemy ... ")
      (if (and (equal? (alchemy '()) '())
               (equal? (alchemy '(lead)) '(gold))
               (equal? (alchemy '(tin)) '(tin))
               (equal? (alchemy '(lead tin lead zinc)) '(gold tin gold zinc))
               (equal? (alchemy '(beer (lead) food)) '(beer (lead) food)))
          (displayln "okay.")
          (error "function alchemy is broken!")))
    #t)

;;; Note in the last test (alchemy '(beer (lead) food)) => (beer (lead) food). That is
;;; alchemy can only change lead that appears at the top level in a list. However, we
;;; can vary the pattern from deep-clone (and deep-anteater) to create a more general
;;; version of alchemy that changes every occurance of lead to gold. So using the definition below,
;;;
;;;    (deep-alchemy '(beer (lead) food)) => (beer (gold) food), and,
;;;    (deep-alchemy 'lead) => gold.

(define (deep-alchemy s)
  (if (pair? s)
      (cons (deep-alchemy (car s)) (deep-alchemy (cdr s)))
      (if (eq? s 'lead)
          'gold
          s)))


;;; And here we test it

(if *run-tests*
    (begin
      (display "Testing deep-alchemy ... ")
      (if (and (equal? (deep-alchemy '()) '())
               (equal? (deep-alchemy '(lead)) '(gold))
               (equal? (deep-alchemy 'lead) 'gold)
               (equal? (deep-alchemy '(lead tin zinc lead)) '(gold tin zinc gold))
               (equal? (deep-alchemy '(beer (lead) food)) '(beer (gold) food))
               (equal? (deep-alchemy 0) 0)
               (equal? (deep-alchemy #\a) #\a)
               (equal? (deep-alchemy "hello") "hello")
               (equal? (deep-alchemy '(lead (lead (lead) lead) lead)) '(gold (gold (gold) gold) gold)))
          (displayln "okay.")
          (error "function alchemy is broken!")))
    #t)

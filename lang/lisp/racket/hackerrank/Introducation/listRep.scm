#lang racket

(define (repeat n x)
  (if (<= n 0)
    '()
    (cons x (repeat (- n 1) x))))

(define (flatmap fn lst)
  (if (null? lst)
    '()
    (append (fn (car lst))
            (flatmap fn (cdr lst)))))

(define (rep-list n lst)
  (flatmap (lambda (x) (repeat n x))
           lst))

; for IO part
(define (read-list-to-eof)
  (define (iter ls)
    (let ([a (read)])
     (if (eof-object? a)
       ls
       (iter (append ls (cons a '()))))))
  (iter '()))

(define (show ls)
  (if (null? ls)
    (void)
    (begin (displayln (car ls))
           (show (cdr ls)))))

(let ([n (read)]
      [ls (read-list-to-eof)])
  (show  (rep-list n ls)))
; sample input
; 3
; 1
; 2
; 3
; 4

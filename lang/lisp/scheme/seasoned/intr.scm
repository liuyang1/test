; How to run this script?
;   racket -f intr.scm
; How to verify code?
;   add displayln to test code

; need this func,which introduced in [The Little Schemer]
(define atom?
  (lambda (x)
    (and (not (pair? x)) (not (null? x)))))

(define isNULL
  (lambda (lat)
    (cond
      ((null? lat) (print "null"))
      (else (print "NOT null")))))

; eq? return #t if v1, v2 and same object
(define member?
  (lambda (a lat)
    (cond
      ((null? lat) #f)
      (else (or (eq? a (car lat))
                (member? a (cdr lat)))))))

; (displayln (member? 1 '(3 2 1)))

;;;

; 是否有两个连续的相同元素
(define two-in-a-row?
  (lambda (lat)
    (cond
      ((null? lat) #f)
      (else
        (or (is-first? (car lat) (cdr lat))
            (two-in-a-row? (cdr lat)))))))

(define is-first?
  (lambda (a lat)
    (cond
      ((null? lat) #f)
      (else (eq? a (car lat))))))

; (displayln (two-in-a-row? '(1 2 3 3 4)))

; 保存状态在preceding里面
(define two-in-a-row-b?
  (lambda (preceding lat)
    (cond
      ((null? lat) #f)
      (else (or (eq? preceding (car lat))
                (two-in-a-row-b? (car lat) (cdr lat)))))))

(define two-in-a-row-b-ex?
  (lambda (lat)
    (cond
      ((null? lat) #f)
      (else (two-in-a-row-b? (car lat) (cdr lat))))))

; (displayln (two-in-a-row-b-ex? '(1 2 3 3 4)))

;;;

(define sum-of-prefixes
  (lambda (tup)
    (cond
      ((null? tup) '())
      (else ..
            (sum-of-prefixes (cdr tup))
            ..))))
; 不能简单的定义sum-of-prefixes 函数，因为没有地方来存储过去的状态信息

; use Additonal arguments when a func needs to know what other args to the func have been like so far
; using ARGs instead of Var
(define sum-of-prefixes-b
  (lambda (sum tup)
    (cond
      ((null? tup) '())
      (else (cons (+ sum (car tup))
                  (sum-of-prefixes-b (+ sum (car tup)) (cdr tup)))))))

(define sum-of-prefixes-b-ex
  (lambda (tup)
    (sum-of-prefixes-b 0 tup)))

; (displayln (sum-of-prefixes-b-ex '(2 1 9 17 0)))

;;;
; scramble 的含义没有看明白

(define (pick n lat)
  (define (one? n) (eq? 1 n))
  (define (sub1 n) (- n 1))
  (cond ((one? n) (car lat))
        (else (pick (sub1 n) (cdr lat)))))

(define (scramble-b tup rev-pre)
  (cond ((null? tup) '())
        (else (let ((rev (cons (car tup) rev-pre)))
               (cons (pick (car tup) rev)
                     (scramble-b (cdr tup) rev))))))

(define (scramble tup) (scramble-b tup '()))

; (displayln (pick 2 '(2 4 3 1 1)))
; (displayln (scramble '(1 2 3 1 2 3 4 1 8 2 10)))

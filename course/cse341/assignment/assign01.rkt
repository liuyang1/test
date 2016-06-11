#lang racket

;;; racket warmup

; 01. define SQUARES recursion function
(define (squares xs)
  (define (square x)
    (* x x))
  (if (null? xs)
    '()
    (cons (square (car xs))
          (squares (cdr xs)))))

(squares (range 10))


; 02. define SQUARES with map
(define (map-squares xs)
  (map (lambda (x) (* x x))
       xs))

(map-squares (range 10))


; 03. define ASSCENDING function
; It's tail recursion function
; AND is short-eval
(define (asscending xs)
  (if (or (null? xs) (null? (cdr xs)))
    #t
    (and (< (car xs) (cadr xs))
         (asscending (cdr xs)))))

(define (test-asscending)
  (and (eq? #t (asscending '()))
       (eq? #t (asscending '(0)))
       (eq? #t (asscending '(1 2 5)))
       (eq? #f (asscending '(1 2 2)))
       (eq? #f (asscending '(2 4 1)))))

(test-asscending)


; 04. let*->let
(define (let*->let xs)
  (define (iter-let pairs body)
    (if (null? pairs)
      (cons 'let (cons  '() body))
      (cons 'let (cons (list (car pairs))
                       (list (iter-let (cdr pairs) body))))))
  (let ([hd1 (car xs)]
        [pairs (cadr xs)]
        [body (cddr xs)])
    (iter-let pairs body)))

(let*->let '(let* ([x 3] [y (+ x 1)]) (+ x y)))
(let*->let '(let* ([x 3] [y (+ x 1)]) (+ x y) (- x y)))
(= (eval '(let* ([x 3] [y (+ x 1)]) (+ x y)) (make-base-namespace))
   (eval (let*->let '(let* ([x 3] [y (+ x 1)]) (+ x y))) (make-base-namespace)))

; 05 


; Extra Credit
; let*->let-all in any syntax
(define (let*->let-all xs)
  (cond ((not (list? xs)) xs)
        ((eq? (car xs) 'let*) (let*->let (map let*->let-all xs)))
        (else (map let*->let-all xs))))

(define (test-let*->let-all sample)
  (let* ([wrap (let*->let-all sample)]
         [ret0 (eval sample (make-base-namespace))]
         [ret1 (eval wrap (make-base-namespace))])
    (displayln sample)
    (displayln wrap)))

(test-let*->let-all '(let* ([x 3] [y (+ x 1)]) (+ x y)))
(test-let*->let-all
  '(define (squid z)
     (let* ([x 3]
            [y x])
       (+ x y z))))
(test-let*->let-all
  '(define (ugly z)
     (let* ([x (let* ((y (* 2 z))) (* 2 y))]
            [y x])
       (let* ([x x])
        (+ x y z)))))

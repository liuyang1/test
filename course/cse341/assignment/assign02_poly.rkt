#lang racket

(require rackunit
         rackunit/text-ui
         rackunit/gui
         )

(define coef car)
(define expo cadr)
(define (build c e) (list c e))

(define head-item car)
(define link-poly cons)

(define *poly-null* '())

(define (poly-neg xs)
  (map (lambda (p) (build (- (coef p)) (expo p)))
       xs))
(define (poly-add xs ys)
  (define (poly-add-list xs ys)
    (let* ([p0 (car xs)]
           [p1 (car ys)]
           [e0 (expo p0)]
           [e1 (expo p1)]
           [c0 (coef p0)]
           [c1 (coef p1)])
      (cond ((= e0 e1) (link-poly (build (+ c0 c1) e0)
                                  (poly-add (cdr xs) (cdr ys))))
            ((> e0 e1) (link-poly p0 (poly-add (cdr xs) ys)))
            (else (link-poly p1 (poly-add xs (cdr ys)))))))
  (cond ((and (null? xs) (null? ys)) *poly-null*)
        ((null? xs) ys)
        ((null? ys) xs)
        (else (poly-add-list xs ys))))

(define (poly-multiply-item p xs)
  (define (poly-multiply-scalar c xs)
    (map (lambda (p) (build (* (coef p) c) (expo p))) xs))
  (define (poly-multiply-shift e xs)
    (map (lambda (p) (build (coef p) (+ e (expo p)))) xs))
  (poly-multiply-shift (expo p)
                       (poly-multiply-scalar (coef p) xs)))

(define (poly-simplfy xs)
  (filter (lambda (p) (not (= 0 (coef p)))) xs))
(define (poly-multiply xs ys)
  (poly-simplfy
    (foldr poly-add
           *poly-null*
           (map (lambda (p) (poly-multiply-item p ys))
                xs))))


(define (poly-cmp xs ys)
  (define (poly-cmp-list xs ys)
    (let* ([p0 (car xs)]
           [p1 (car ys)]
           [e0 (expo p0)]
           [e1 (expo p1)]
           [c0 (coef p0)]
           [c1 (coef p1)])
      (cond ((> e0 e1) 1)
            ((< e0 e1) -1)
            (else (cond ((> c0 c1) 1)
                        ((< c0 c1) -1)
                        (else (poly-cmp (cdr xs) (cdr ys))))))))
  (cond ((and (null? xs) (null? ys)) 0)
        ((null? ys) 1)
        ((null? xs) -1)
        (else (poly-cmp-list xs ys))))
(define (poly-eq? xs ys)
  (= 0 (poly-cmp xs ys)))

(define (poly->code-item p var)
  (let ([c (coef p)]
        [e (expo p)])
    (cond ((= c 0) 0)
          ((= e 0) c)
          ((and (= e 1) (= c 1) var))
          ((= e 1) `(* ,c ,var))
          ((= c 1) `(expt ,var ,e))
          (else `(* ,c (expt ,var ,e))))))
(define (poly->code poly var)
  (define (add-add-symbol xs)
    (let ((len (length xs)))
     (cond ((= 0 len) 0)
           ((= 1 len) (car xs))
           (else (cons '+ xs)))))
  (add-add-symbol
    (map (lambda (p) (poly->code-item p var)) poly)))


;;; Test code ;;;
(define (unit-test-poly-multiply a b expect)
  (let ((ret (poly-multiply a b)))
   (check-true (poly-eq? ret expect))))

(define (eval-poly-x formula v)
  (eval `(let ([x ,v])
          ,formula)
        (make-base-namespace)))

(define test-poly-multiply
  (test-suite
    "Test for poly"
    (test-case
      "Test poly-multiply"
      (unit-test-poly-multiply '((3 0)) '((1 1))
                               '((3 1)))
      (unit-test-poly-multiply '((1 1)) '((3 0))
                               '((3 1)))
      (unit-test-poly-multiply '((1 3) (1 2) (1 1) (1 0)) '((1 1) (-1 0))
                               '((1 4) (-1 0)))
      (unit-test-poly-multiply '((-3 4) (1 1) (5 0)) '((0 0))
                               *poly-null*)
      (unit-test-poly-multiply '((0 0)) '((1 2))
                               *poly-null*)
      (unit-test-poly-multiply '((1 3) (1 1) (-1 0)) '((-5 0))
                               '((-5 3) (-5 1) (5 0)))
      (unit-test-poly-multiply '((-10 2) (100 1) (5 0)) '((1 999) (-1 7) (1 1) (3 0))
                               '((-10 1001) (100 1000) (5 999) (10 9) (-100 8) (-5 7) (-10 3) (70 2) (305 1) (15 0))))
    (test-case
      "Test poly->code"
      (check-equal? (poly->code '((1 3) (5 2) (7 1) (10 0)) 'x)
                    '(+ (expt x 3) (* 5 (expt x 2)) (* 7 x) 10))
      (check-equal? (poly->code '((1 1) (-10 0)) 'x)
                    '(+ x -10))
      (check-equal? (poly->code '((1 1)) 'x)
                    'x)
      (check-equal? (poly->code '((10 0)) 'x)
                    10)
      (check-equal? (poly->code '((1 0)) 'x)
                    1)
      (check-equal? (poly->code '() 'x)
                    0))
    (test-case
      "Test poly->code instance"
      (let ([p1 '((1 3) (1 2) (1 1) (1 0))]
            [p2 '((1 1) (-1 0))]
            [*x* 4])
        (check-equal?
          (eval-poly-x  (poly->code
                          (poly-multiply p1 p2) 'x)
                        *x*)
          (* (eval-poly-x (poly->code p1 'x) *x*)
             (eval-poly-x (poly->code p2 'x) *x*)))))))

(run-tests test-poly-multiply)
; (test/gui test-poly-multiply #:wait? #t)

#lang racket

(define ones (lambda () (cons 1 ones)))

(define nats
  (letrec ([f (lambda (x) (cons x (lambda () (f (+ x 1)))))])
    (lambda () (f 1))))

(define powers-of-two
  (letrec ([f (lambda (x) (cons x (lambda () (f (* 2 x)))))])
    (lambda () (f 2))))

(define (take n s)
  (if (= n 0)
    (car (s))
    (take (- n 1) (cdr (s)))))

(define (test-stream s n)
  (for-each (lambda (i) (displayln (take i s)))
            (range n)))

(test-stream ones 3)
(test-stream nats 5)
(test-stream powers-of-two 5)

(define (until stream pred)
  (letrec ([f (lambda (stream ans)
                (let ([pr (stream)])
                  (if (pred (car pr))
                    ans
                    (f (cdr pr) (+ ans 1)))))])
    (f stream 1)))

(until powers-of-two (lambda (x) (> x 30)))

(define (stream-maker fn arg)
  (letrec ([f (lambda (x)
            (cons x (lambda () (f (fn x arg)))))])
    (lambda () (f arg))))

(define ones-1 (stream-maker (lambda (x y) 1) 1))
(define nats-1 (stream-maker + 1))
(define powers-of-two-1 (stream-maker * 2))
(test-stream ones-1 3)
(test-stream nats-1 5)
(test-stream powers-of-two-1 5)

(define (square a) (* a a))

(define (smallest-divisor n)
  (find-divisor n 2))

(define (find-divisor n test-divisor)
  (cond ((> (square test-divisor) n)    n)
        ((divides? test-divisor n)      test-divisor)
        (else (find-divisor n (+ test-divisor 1)))))

(define (divides? a b)
  (= (remainder b a) 0))

(define (test-smallest-diviso n)
  (displayln (smallest-divisor n)))

(test-smallest-diviso 199)
(test-smallest-diviso 1999)
(test-smallest-diviso 19999)

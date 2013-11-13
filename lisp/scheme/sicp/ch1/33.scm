(define (filt-acc comb null term a next b filt)
  (define (acc v)
    (if (>= v b) null
      (if (filt v)
        (comb (term v) (acc (next v)))
        (acc (next v)))))
  (acc a))

(define (inc a) (+ a 1))
(define (self a) a)
(define (TRUE a) true)

(define (find-divisor n test-divisor)
  (define (square a) (* a a))
  (define (next-divisor val) (if (<= val 2) (+ val 1) (+ val 2)))
  (define (divides? a b) (= (remainder b a) 0))
  (cond ((> (square test-divisor) n)    n)
        ((divides? test-divisor n)      test-divisor)
        (else (find-divisor n (next-divisor test-divisor)))))

(define (prime? n)
  (= n (find-divisor n 2)))

(displayln (filt-acc + 0 self 1 inc 10 TRUE))
(displayln (filt-acc + 0 self 2 inc 10 prime?))

(define (prod-prime n)
  (define (gcd? a) (= (gcd a n) 1))
  (filt-acc * 1 self 2 inc n gcd?))
(displayln (prod-prime 7))

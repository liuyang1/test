(define (accumulate-rec combiner null-value term a next b)
  (if (> a b) null-value
    (combiner (term a) 
              (accumulate-rec combiner null-value term (next a) next b))))

(define (accumulate-iter combiner null-value term a next b)
  (define (ip a result)
    (if (> a b) result (ip (next a) (combiner (term a) result))))
  (ip a null-value))

(define (accumulate combiner null-value term a next b)
  (accumulate-iter combiner null-value term a next b))

(define (sum term a next b)
  (accumulate + 0 term a next b))

(define (product term a next b)
  (accumulate * 1 term a next b))


(define (self a) a)
(define (inc a) (+ a 1))

(define (fact n)
  (product self 1 inc n))

(displayln (fact 5))
(displayln (sum self 1 inc 10))

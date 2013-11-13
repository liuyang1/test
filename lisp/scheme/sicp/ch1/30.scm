(define (sum term a next b)
  (define (iter a result)
    (if (> a b) result 
      (iter (next a) (+ result (term a)))))
  (iter a 0))

(define (func a) a)
(define (inc a) (+ a 1))
(displayln (sum func 1 inc 10))

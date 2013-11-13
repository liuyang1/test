(define (product-iter func a next b)
  (define (iter a result)
    (if (> a b) result
      (iter (next a) (* result (func a)))))
  (iter a 1))

(define (product-rec func a next b)
  (if (> a b) 1 (* (func a) (product-rec func (next a) next b))))

(define (func-pi n)
  (if (even? n) (/ n (+ n 1.0)) (/ (+ n 1.0) n)))

(define (pi-rec)
  (* 4 (product-rec func-pi 2 inc 10000000)))

(define (pi-iter)
  (* 4 (product-iter func-pi 2 inc 10000000)))

(displayln (pi-rec))
(displayln (pi-iter))

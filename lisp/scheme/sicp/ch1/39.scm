(define (lambert n d k)
  (define (cont val k)
    (if (= k 0) val
      (cont (/ (n k) (- (d k) val)) (- k 1))))
  (cont 0.0 k))

(define (tan-cf x k)
  (define (ni i) (expt x i))
  (define (di i) (- (* 2 i) 1))
  (lambert ni di k))


(displayln (tan-cf 0.785 100))

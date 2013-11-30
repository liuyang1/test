(define (cont-frac ni di k)
  (define (cont val k)
    (if (= k 0) val
      (cont (/ (ni k) (+ (di k) val)) (- k 1))))
  (cont 0.0 k))

(define (cont-frac-rec ni di k)
  (define (cont i)
    (if (= i k) (/ (ni k) (di k))
      (/ (ni i) (+ (di i) (cont (+ i 1))))))
  (cont 1))

(displayln (cont-frac (lambda (x) 1) (lambda (x) 1) 100))
(displayln (cont-frac-rec (lambda (x) 1.0) (lambda (x) 1) 100))

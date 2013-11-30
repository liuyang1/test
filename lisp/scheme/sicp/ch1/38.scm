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

(define (e-ni i) 1)
(define (e-ki i) (if (= 0 (remainder (+ i 1) 3)) (* 2 (/ (+ i 1) 3)) 1))

(displayln (+ 2 (cont-frac e-ni e-ki 10000)))

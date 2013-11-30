(define (iter-impr close? improve)
  (define (ip x) 
    (let ((next (improve x)))
     (if (close? next x) next (ip next))))
  ip)

(define (mysqrt x)
  (define (close? v1 v2) (< (abs (- v1 v2)) 0.001))
  (define (improve guess) (/ (+ guess (/ x guess)) 2.0))
  ((iter-impr close? improve) x))

(displayln (mysqrt 2))

(define (fixed f first)
  (define (close? v1 v2) (< (abs (- v1 v2)) 0.001))
  (define (improve guess) (f guess))
  ((iter-impr close? improve) first))

(displayln (fixed cos 1.0))

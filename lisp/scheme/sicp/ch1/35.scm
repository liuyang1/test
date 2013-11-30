(define (fixed f first)
  (define tolerance 0.00001)
  (define (close? v1 v2) (< (abs (- v1 v2)) tolerance))
  (define (try guess)
    (let ((next (f guess)))
     (if (close? guess next) next
       (try next))))
  (try first))

(displayln (fixed cos 1.0))
(displayln (fixed (lambda (x) (+ 1 (/ 1 x))) 1.0))

(define (fixed f first)
  (define tolerance 0.00001)
  (define (close? v1 v2) (< (abs (- v1 v2)) tolerance))
  (define (try guess count)
    (let ((next (f guess)))
     (if (close? guess next) (begin (displayln count) next)
       (begin (try next (+ count 1))))))
  (try first 0))

(define (func x) (/ (log 1000) (log x)))
; high order func
(define (damp func)
  (define (proc x) (/ (+ x  (func x)) 2))
  proc)

(displayln (fixed func 5))
(displayln (fixed (damp func) 5))

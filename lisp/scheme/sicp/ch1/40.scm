(define (cubic a b c) 
  (lambda (x) (+ (* x (+ (* x (+ x a)) b)) c)))

(define (fixed f first)
  (define tolerance 0.00001)
  (define (close? v1 v2) (< (abs (- v1 v2)) tolerance))
  (define (try guess count)
    (let ((next (f guess)))
     (if (close? guess next) next
       (begin (try next (+ count 1))))))
  (try first 0))

(define (newtons func guess)
  (define dx 0.0001)
  (define (deriv g)
    (lambda (x) (/ (- (g (+ x dx)) (g x)) dx)))
  (define (newton-trans g)
    (lambda (x) (- x (/ (g x) ((deriv g) x)))))
  (fixed (newton-trans func) guess))

(displayln  (newtons (cubic 1 1 1) 1))

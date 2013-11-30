(define (compose f g) (lambda (x) (f (g x))))

(define (repeat f n)
  (if (= n 1) f (compose f (repeat f (- n 1)))))

(define (square x) (* x x))

(displayln ((repeat square 2) 5))

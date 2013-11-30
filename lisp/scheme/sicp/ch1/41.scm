(define (double func)
  (lambda (x) (func (func x))))

(define (inc x) (+ x 1))

(displayln ((double inc) 5))
(displayln (((double (double double)) inc) 5))

; Church 
(define zero (lambda (f) (lambda (x) x)))
(define (add-1 n) (lambda (f) (lambda (x) (f ((n f) x)))))

(define one (lambda (f) (lambda (x) (f (x)))))
(define two (lambda (f) (lambda (x) (f (f (x))))))
(define (plus m n) (lambda (f) (lambda (x) (m ((n f) x)))))

(displayln zero)
(displayln (add-1 zero))
(displayln one)
(displayln (plus one two))

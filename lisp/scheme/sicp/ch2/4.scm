(define (mycons x y) (lambda (m) (m x y)))
(define (mycar z) (z (lambda (p q) p)))
(define (mycdr z) (z (lambda (p q) q)))

(define t (mycons 1 2))
(displayln (mycar t))
(displayln (mycdr t))

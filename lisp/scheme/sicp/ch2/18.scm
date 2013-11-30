(define (myreverse lst)
  (define (ip in out)
    (if (null? in) out
      (ip (cdr in) (cons (car in) out))))
  (ip lst '()))

(displayln (myreverse '()))
(displayln (myreverse (list 0)))
(displayln (myreverse (list 0 1)))

(define (myreverse-rec lst)
  (if (null? lst) '()
    (append (myreverse-rec (cdr lst)) (list (car lst)))))

(displayln (myreverse-rec '()))
(displayln (myreverse-rec (list 0)))
(displayln (myreverse-rec (list 0 1)))


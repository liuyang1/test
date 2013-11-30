(define (square x) (* x x))

(define (square-list items)
  (define (iter things answer)
    (if (null? things) answer
      (iter (cdr things) (append answer (list (square (car things)))))))
  (iter items '()))

(define lst (list 0 1 2 3 4 5))
(displayln (square-list lst))

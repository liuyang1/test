(define (square x) (* x x))

(define (square-list-0 items)
  (if (null? items) '()
    (cons (square (car items)) (square-list-0 (cdr items)))))

(define (square-list-1 items)
  (map square items))

(define lst (list 0 1 2 3 4 5))
(displayln (square-list-0 lst))
(displayln (square-list-1 lst))

(define (accumulate op initial sequence)
  (if (null? sequence) initial
    (op (car sequence)
        (accumulate op initial (cdr sequence)))))

(define (count-leaves tree)
  (accumulate + 0 (map
                    (lambda (x) (if (list? x) (count-leaves x) 1))
                    tree)))

(define *test-tree* (list 1 (list 3 2 4)))
(displayln (count-leaves *test-tree*))
(displayln (count-leaves (list (list 1 (list 2 3) 4) 5)))

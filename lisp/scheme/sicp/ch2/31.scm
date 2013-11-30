(define (tree-map func tree)
  (define (helper tree)
    (if (not (list? tree)) (func tree)
      (map helper tree)))
  (helper tree))

(define (square x) (* x x))
(define (square-tree tree)
  (tree-map square tree))

(define lst (list 1 (list 2 (list 3 4) 5)))
(displayln lst)
(displayln (square-tree lst))

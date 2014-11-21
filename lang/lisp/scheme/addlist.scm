(define (add-one-list val lst)
  (if (null? lst) '()
    (cons (list val (car lst)) (add-one-list val (cdr lst)))))

(define (add-list lst0 lst1)
  (if (null? lst0) '()
    (append (add-one-list (car lst0) lst1)
            (add-list (cdr lst0) lst1))))

(display (add-one-list 3 '(1 2)))
(newline)
(display (add-list '(3 4) '(1 2)))
(display (add-list '(3 4 5) '(1 2 3)))

(define (add-list-1 lst0 lst1)
  (for*/list ((i (in-list lst0)) (j (in-list lst1)))
             (list i j)))

(newline)
(display (add-list-1 '(1 2 3) '(100 200)))

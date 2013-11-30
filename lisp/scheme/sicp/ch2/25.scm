(define lst0 '(1 3 (5 7) 9))
(define lst1 '((7)))
(define lst2 '(1 (2 (3 (4 (5 (6 7)))))))

(displayln (car (cdr (car (cdr (cdr lst0))))))
(displayln (cadr (caddr lst0)))
(displayln (car (car lst1)))
(displayln (car (cdr (car (cdr (car (cdr (car (cdr (car (cdr (car (cdr lst2)))))))))))))
(displayln (cadr (cadr (cadr (cadr (cadr (cadr lst2)))))))

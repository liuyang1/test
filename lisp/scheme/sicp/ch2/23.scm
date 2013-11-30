(define (my-for-each func lst)
  (if (null? lst) '()
    (begin (func (car lst)) (my-for-each func (cdr lst)))))

(my-for-each (lambda (x) (display x) (newline))
             (list 1 2 3))

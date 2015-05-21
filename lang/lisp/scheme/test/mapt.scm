(define map-t
  (lambda (f ls)
    (if (null? ls) '()
      (cons (f (car ls)) (map-t f (cdr ls))))))

(display (let ((ls '(1 2 3 4)))
          (map-t (lambda (x) (cons x ls))
                 ls)))

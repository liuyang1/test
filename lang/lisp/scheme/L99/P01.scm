(define mylast ls
  (if (null? (cdr ls))
    (car ls)
    (mylast (cdr ls))))

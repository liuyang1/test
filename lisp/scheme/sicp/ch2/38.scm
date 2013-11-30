; font italic
(define (accumulate op initial sequence)
  (if (null? sequence) initial
    (op (car sequence)
        (accumulate op initial (cdr sequence)))))

(define (my-fold-left op init seq)
  (define (iter result rest)
    (if (null? rest) result
      (iter (op result (car rest)) (cdr rest))))
  (iter init seq))

(define my-fold-right accumulate)

(displayln (my-fold-right / 1 (list 1 2 3)))
(displayln (my-fold-left / 1 (list 1 2 3)))
(displayln (my-fold-right list '() (list 1 2 3)))
(displayln (my-fold-left list '() (list 1 2 3)))

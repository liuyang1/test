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

(define (reverse0 seq)
  (my-fold-right (lambda (x y) (append y (list x))) '() seq))

(define (reverse1 seq)
  (my-fold-left (lambda (x y) (cons y x)) '() seq))

(define *test-obj* (list 1 2 3))
(displayln (reverse1 *test-obj*))
(displayln (reverse0 *test-obj*))

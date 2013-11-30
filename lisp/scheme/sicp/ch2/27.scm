(define (myreverse lst)
  (define (ip in out)
    (if (null? in) out
      (ip (cdr in) (cons (car in) out))))
  (ip lst '()))

(define (myreverse-rec lst)
  (if (null? lst) '()
    (append (myreverse-rec (cdr lst)) (list (car lst)))))

(define x (list (list 1 2) (list 3 4)))
(displayln (myreverse x))

(define (deep-reverse lst)
  (cond ((not (list? lst)) lst)
        ((null? lst) '())
        (else (append (deep-reverse (cdr lst)) (list (deep-reverse (car lst)))))))
(displayln (deep-reverse (list 1 2 3)))
(displayln (deep-reverse x))

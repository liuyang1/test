(define (accumulate op initial sequence)
  (if (null? sequence) initial
    (op (car sequence)
        (accumulate op initial (cdr sequence)))))

(define (mymap p sequence)
  (accumulate (lambda (head tail) (cons (p head) tail))
              '()
              sequence))
(define (myappend seq0 seq1)
  (accumulate cons seq1 seq0))
(define (mylength seq)
  (accumulate (lambda (head tail) (+ 1 tail))
              0
              seq))

(define (square x) (* x x))

(define lst '(1 2 3))
(define lst1 '(4 5 6))
(displayln lst)
(displayln (mymap square lst))
(displayln (myappend lst lst1))
(displayln (mylength lst))
(displayln (mylength (myappend lst lst)))

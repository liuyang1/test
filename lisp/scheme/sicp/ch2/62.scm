(define (element-of-set? x set)
  (cond ((null? set) false)
        ((= x (car set)) true)
        ((< x (car set)) false)
        (else (element-of-set? x (cdr set)))))

(define (union-set set0 set1)
  (cond ((null? set0) set1)
        ((null? set1) set0)
        ((= (car set0) (car set1))
         (cons (car set0) (union-set (cdr set0) (cdr set1))))
        ((< (car set0) (car set1))
         (cons (car set0) (union-set (cdr set0) set1)))
        (else
          (cons (car set1) (union-set set0 (cdr set1))))))

(displayln (union-set '(1 3) '(2 3 6)))

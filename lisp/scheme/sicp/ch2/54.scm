; 为了处理两个列表其中一个为空,或者都为空的情况,其实还是挺麻烦的.
(define (my-equal? a b)
  (let ((heada (car a))
        (headb (car b)))
  (if (or (and (number? heada) (number? headb) (= heada headb))
          (and (symbol? heada) (symbol? headb) (eq? heada headb))
          (and (list? heada) (list? headb)
               (or (and (null? heada) (null? headb)) (my-equal? heada headb))))
    (or (and (null? (cdr a)) (null? (cdr b)))
        (and (not (null? (cdr a))) (not (null? b)) (my-equal? (cdr a) (cdr b))))
    #f)))

(displayln (my-equal? '(this is a list) '(this (is a) list a)))
(displayln (my-equal? '(this is a list) '(this is a list)))

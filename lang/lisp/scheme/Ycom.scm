(define (Y F)
  ((lambda (x) (x x))
   (lambda (g)
     (F (lambda args (apply (g g) args))))))

;(display ((Y F) 5))
;(newline)

(define (fact self)
  (lambda (n)
  (if (= n 0) 1
    (* n ((self self) (- n 1))))))

(define fac (fact fact))
(display (fac 5))
(newline)

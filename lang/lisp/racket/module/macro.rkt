#lang racket

(define-syntax my-if
  (syntax-rules (then else)
                [(my-if e1 then e2 else e3)
                 (if e1 e2 e3)]))

(define (my-divide x y)
  (my-if (= y 0)
         then
         (displayln "wrong dividor")
         else
         (/ x y)))

(my-divide 3 2)
(my-divide 3 0)

(define-syntax comment-out
  (syntax-rules ()
                [(comment-out e1 e2) e2]))

(comment-out (displayln "first line")
             (displayln "sencond line"))

(define-syntax my-delay
  (syntax-rules ()
                [(my-delay e)
                 (mcons #f (lambda () e))]))

(define-syntax my-force
  (syntax-rules ()
                [(my-force e)
                 (let ([x e])
                   (if (mcar x)
                     (mcdr x)
                     (begin (set-mcar! x #t)
                            (set-mcdr! x ((mcdr x)))
                            (mcdr x))))]))

(define (my-mult x y-promise)
  (cond [(= x 0) 0]
        [(= x 1) (my-force y-promise)]
        [#t (+ (my-force y-promise) (my-mult (- x 1) y-promise))]))

(my-mult 3
         (my-delay 5))

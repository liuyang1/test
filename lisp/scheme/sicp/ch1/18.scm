(define (double a) (* a 2))
(define (halve a) (/ a 2))


(define (asterisk a b)
  (define (ip m a b)
    (cond ((< a 0)      (- (ip m (- a) b)))
          ((< b 0)      (- (ip m a (- b))))
          ((= b 0)      m)
          ((even? b)    (ip m (double a) (halve b)))
          (else         (ip (+ m a) a (- b 1))))) 
  (ip 0 a b))

; test
(displayln (asterisk 2 3))
(displayln (asterisk -2 3))
(displayln (asterisk 2 -3))
(displayln (asterisk -2 -3))

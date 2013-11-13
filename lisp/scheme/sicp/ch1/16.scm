(define (fast-expt b n)
  (define (process a b n)
    (cond ((= n 0)      a)
          ((even? n)    (process a (* b b) (/ n 2)))
          (else         (process (* a b) b (- n 1)))))
  (process 1 b n))

; for test
(display (fast-expt 2 3))
(newline)

(define (square a) (* a a))
(define (expmod base expr m)
  (cond ((= expr 0) 1)
        ((even? expr)
         (remainder (square (expmod base (/ expr 2) m))
                    m))
        (else
          (remainder (* base (expmod base (- expr 1) m))
                     m))))

; Carmichael
(define (carmichael? n)
  (define (ismod? a n)
    (= (expmod a n n) a))
  (define (loopcheck a n)
    (cond ((= a n) true)
          ((ismod? a n) (loopcheck (+ a 1) n))
          (else false))
    )
  (loopcheck 2 n))

(displayln (carmichael? 100))
(displayln (carmichael? 27))
(displayln (carmichael? 97))

; carmichael number is not prime number,
; but can pass fermat-test
(displayln (carmichael? 561))
(displayln (carmichael? 1105))
(displayln (carmichael? 1729))
(displayln (carmichael? 2465))
(displayln (carmichael? 2821))
(displayln (carmichael? 6601))

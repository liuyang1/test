(define (square a) (* a a))

(define (smallest-divisor n)
  (find-divisor n 2))

(define (find-divisor n test-divisor)
  (cond ((> (square test-divisor) n)    n)
        ((divides? test-divisor n)      test-divisor)
        (else (find-divisor n (next-origin test-divisor)))))

(define (next-origin val)
  (+ val 1))

(define (next val)
  (if (<= val 2)
    (+ val 1)
    (+ val 2)))

(define (divides? a b)
  (= (remainder b a) 0))

(define (test-smallest-diviso n)
  (displayln (smallest-divisor n)))

(define (prime? n)
  (= n (smallest-divisor n)))

(define (timed-prime-test n)
  (start-prime-test n (current-inexact-milliseconds)))

(define (start-prime-test n start-time)
  (if (prime? n)
    (report-prime n start-time (current-inexact-milliseconds))
    #f)) 

(define (report-prime n start-time end-time)
  (display n)
  (display " ")
  (display (- end-time start-time))
  (newline))


(define (search-for-primes begin-value)
  (if (not (timed-prime-test begin-value))
    (search-for-primes (+ begin-value 1))
    #t))

(search-for-primes 1000)
(search-for-primes (* 1000 1000))
(search-for-primes (* 1000 1000 1000))

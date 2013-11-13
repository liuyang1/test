(define (square a) (* a a))

(define (isord2? a n)
  (and (not (= a 1)) (not (= (- a 1) n)) (= (remainder (square a) n) 1)))

(define (expmod base expr m)
  (cond ((= expr 0) 1)
        ((even? expr)
         (remainder (square (expmod base (/ expr 2) m))
                    m))
        (else
          (remainder (* base (expmod base (- expr 1) m))
                     m))))

(define (isord? n)
  (define (loopcheck a n)
    (cond ((= a (- n 1)) false)
          ((not (= (remainder (square a) n) 1)) 
           (loopcheck (+ a 1) n))
          (else a)))
  (loopcheck 2 n))

(define (miller-rabin n)
  (define (try-it a)
    (and (not (isord2? a n)) (= (expmod a (- n 1) n) 1)))
  (try-it (+ 1 (random (- n 1)))))

(define (fast-prime? n times)
  (cond ((= times 0) true)
        ((miller-rabin n) (fast-prime? n (- times 1)))
        (else false)))

(define (prime? n)
  (fast-prime? n 100))

(define (timed-prime-test n)
  (start-prime-test n (current-inexact-milliseconds)))

(define (start-prime-test n start-time)
  (if (prime? n)
    (report-prime n start-time (current-inexact-milliseconds))
    #f)) 

(define (report-prime n start-time end-time)
  (displayln n))

(define (search-for-primes-one begin-value)
  (if (not (timed-prime-test begin-value))
    (search-for-primes-one (+ begin-value 1))
    begin-value))

(define (search-for-primes-n begin-value n)
  (if (not ( = n 0))
    (search-for-primes-n (+ 1 (search-for-primes-one begin-value)) (- n 1))
    #f))

(search-for-primes-n (expt 10 1) 10)

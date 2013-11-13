(define (square a)
  (* a a))

(define (expmod base expr m)
  (cond ((= expr 0) 1)
        ((even? expr)
         (remainder (square (expmod base (/ expr 2) m))
                    m))
        (else
          (remainder (* base (expmod base (- expr 1) m))
                     m))))

(define (fermat-test n)
  (define (try-it a)
    (= (expmod a n n) a))
  (try-it (+ 1 (random (- n 1)))))

(define (fast-prime? n times)
  (cond ((= times 0) true)
        ((fermat-test n) (fast-prime? n (- times 1)))
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
  (display n)
  (display " ")
  (display (- end-time start-time))
  (newline))

(define (search-for-primes-one begin-value)
  (if (not (timed-prime-test begin-value))
    (search-for-primes-one (+ begin-value 1))
    begin-value))

(define (search-for-primes-n begin-value n)
  (if (not ( = n 0))
    (search-for-primes-n (+ 1 (search-for-primes-one begin-value)) (- n 1))
    #f))

(search-for-primes-n (expt 10 3) 3)
(search-for-primes-n (expt 10 4) 3)
(search-for-primes-n (expt 10 5) 3)
(search-for-primes-n (expt 10 6) 3)
(search-for-primes-n (expt 10 7) 3)
(search-for-primes-n (expt 10 8) 3)

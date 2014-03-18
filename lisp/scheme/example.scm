(define (f return) (return 2) 3)

(display (f (lambda (x) x)))
(newline)

(display (call/cc f))
(newline)
; when F is applying the parameter to 2 forces execution of the program
;   to jump to the point where call/cc was called
; and causes call/cc to return the value 2

(define (search-zero lst)
  (call/cc
    (lambda (return)
      (for-each (lambda (element)
                  (if (zero? element)
                    (return 'found-zero)
                    (printf "~a~%" element)))
                lst)
      #f)))

(displayln (search-zero '(-3 -2 -1 0 1 2 3)))

(define (generate-one lst)
  (define (control-state return)
    (for-each (lambda (element)
                (call/cc (lambda (resume-here)
                           (set! control-state resume-here)
                           (return element))))
              lst)
    (return 'end))
  (define (generator) (call/cc control-state))
  generator)

(define generate-digit
  (generate-one '(0 1 2)))

(displayln (generate-digit))
(displayln (generate-digit))
(displayln (generate-digit))
(displayln (generate-digit))
(displayln (generate-digit))

(define dish #f)
(define (put! fruit) (set! dish fruit))
(define (get!) (let ([fruit dish]) (set! dish #f) fruit))
(define (consumer do-other)
  (let loop ()
   (when dish
     (printf "C: get a ~a~%" (get!))
     (set! do-other (call/cc do-other))
     (loop))))

(define (producer do-other)
  (for-each (lambda (fruit)
              (put! fruit)
              (printf "P: put a ~a~%" fruit)
              (set! do-other (call/cc do-other)))
            '("apple" "peach" "grape")))

(producer consumer)

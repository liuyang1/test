; directly use reduce function
; it's so easy to build MAX function.
; better than pattern-match style.
(define (max ns)
  (define (reduce f xs)
    (if (null? xs) '() (foldl f (car xs) (cdr xs))))
  (define (max0 a b) (if (> a b) a b))
  (reduce max0 ns))

(displayln (max '(0 1 2 3)))

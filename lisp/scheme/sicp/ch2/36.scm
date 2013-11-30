(define (accumulate op initial sequence)
  (if (null? sequence) initial
    (op (car sequence)
        (accumulate op initial (cdr sequence)))))

(define (accumulate-n op init seqs)
  (if (null? (car seqs)) '()
    (cons (accumulate op init (map car seqs))
          (accumulate-n op init (map cdr seqs)))))

(define *test-obj* (list (list 1 2 3)
                         (list 4 5 6)
                         (list 7 8 9)
                         (list 10 11 12)))
(displayln *test-obj*)
(displayln (accumulate-n + 0 *test-obj*))

(define (accumulate op initial sequence)
  (if (null? sequence) initial
    (op (car sequence)
        (accumulate op initial (cdr sequence)))))

(define (accumulate-n op init seqs)
  (if (null? (car seqs)) '()
    (cons (accumulate op init (map car seqs))
          (accumulate-n op init (map cdr seqs)))))

(define (dot-product v w)
  (accumulate + 0 (map * v w)))

(define (matrix-*-vector m v)
  (map (lambda (x) (dot-product x v)) m))

(define (transpose mat)
  (accumulate-n cons '() mat))

(define (matrix-*-matrix m n)
  (let ((cols (transpose n)))
   (map (lambda (x) 
          (map (lambda (y)(dot-product x y)) 
               cols)) 
        m)))

(define *test-vector0* (list 1 2 3))
(define *test-vector1* (list 3 2 1))
; (displayln (dot-product *test-vector0* *test-vector1*))

(define *test-matrix* (list (list 1 2) (list 1 0)))
(define *test-vector* (list 1 2))
(displayln (matrix-*-vector *test-matrix* *test-vector*))

(displayln *test-matrix*)
(displayln (transpose *test-matrix*))

(displayln (matrix-*-matrix *test-matrix* *test-matrix*))

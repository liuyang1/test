(define (flatmap proc seq)
  (accumulate append '() (map proc seq)))

(define (enumerate-interval low high)
  (if (> low high) '()
    (cons low (enumerate-interval (+ low 1) high))))

(define (accumulate op initial sequence)
  (if (null? sequence) initial
    (op (car sequence)
        (accumulate op initial (cdr sequence)))))

(define (unique-3-pairs n)
  (flatmap (lambda (i)
             (flatmap (lambda (j)
                        (map (lambda (k) (list i j k))
                             (enumerate-interval 1 (- j 1))))
                  (enumerate-interval 1 (- i 1))))
           (enumerate-interval 1 n)))

(define (sum-3 n)
  (define (isSum3? lst)
    (= n (accumulate + 0 lst)))
  (filter isSum3? (unique-3-pairs (- n 3))))

; (map displayln (unique-3-pairs 5))
; (map displayln (map (lambda (x) (accumulate + 0 x)) (unique-3-pairs 5)))
(map displayln (sum-3 10))

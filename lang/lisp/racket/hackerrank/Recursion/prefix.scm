#lang racket

(define (prefix as bs)
  (define (help as bs acc)
    (cond ((or (null? as) (null? bs)) (list as bs acc))
          ((equal? (car as) (car bs)) (help (cdr as) (cdr bs) (cons (car as) acc)))
          (else (list as bs acc))))
  (help as bs '()))

(define (prefix-str as bs)
  (map list->string (prefix (string->list as)
                            (string->list bs))))

;;;
(define (string-rev s)
  (list->string (reverse (string->list s))))

(define (show-len-str s)
  (let ((len (string-length s)))
   (if (= 0 len)
     (displayln len)
     (fprintf (current-output-port) "~a ~a\n" (string-length s) s))))

(let ([as (read-line)]
      [bs (read-line)])
  (let* ((rt (prefix-str as bs))
         ; NOTICE: return rev for speed, here reverse it back
         (pre (string-rev (caddr rt)))
         (t0 (car rt))
         (t1 (cadr rt)))
    (show-len-str pre)
    (show-len-str t0)
    (show-len-str t1)))

#lang racket

;;; Run-Length Encoding

(define (rle s)
  (define (stat ss ch cnt acc)
    (if (null? ss)
      (cons (cons ch cnt) acc)
      (let ((h (car ss))
            (t (cdr ss)))
        (cond ((equal? h ch) (stat t h (+ cnt 1) acc))
              (else (stat t h 1 (cons (cons ch cnt) acc)))))))
  (stat (cdr s) (car s) 1 '()))

(define (group s)
  (foldl (lambda (ch ss)
           (cond ((null? ss) (list (list ch)))
                 ((equal? ch (car (car ss)))
                  (cons (cons ch (car ss)) (cdr ss)))
                 (else (cons (list ch) ss))))
         '()
         s))

; Hashkell style
; runLengthEncoding = map (length &&& head) . group
(define (rle-ex s)
  (map (lambda (x) (cons (car x) (length x)))
       (group s)))

; format it back to string
(define (fmt rp)
  (define (fmt-one pair)
    (let ((ch (car pair))
          (cnt (cdr pair)))
      (if (= cnt 1)
        (make-string 1 ch)
        (string-append (~a ch) (~a cnt)))))
  (define (hl rp)
    (if (null? rp)
      ""
      (string-append (hl (cdr rp))
                     (fmt-one (car rp)))))
  (hl rp))

(define (fmt-rle s)
  (fmt (rle-ex (string->list s))))

;;; test
(define (test-case)
  (string=? 
    (fmt-rle  "aaabaaaaccaaaaba")
            "a3ba4c2a4ba")
)

(test-case)
;;; IO part

; (let ([s (read-line)])
;  (displayln (fmt-rle s)))

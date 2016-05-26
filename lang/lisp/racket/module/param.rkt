#lang racket

(define (double x) (* 2 x))
(define (triple x) (* 3 x))

(define func (make-parameter double))

; NOTICE: func is a PARAM, need one time apply with () to get it
(define (apply-func x)
  ((func) x))

(define (nest-func x)
  (+ 2 (apply-func x) ))

; we could change param in PARAMTERIZE scope
(apply-func 4 )
(parameterize ([func triple])
              (apply-func 4))
(apply-func 4 )

; This change even affect nest-level
(nest-func 4)
(parameterize ([func triple])
              (nest-func 4))


; compare behaviour SET!
(define funk double)
(define (apply-funk x)
  (funk x))
(apply-funk 4)
(set! funk triple)
(apply-funk 4)
; cannot auto change back, it affect whole scope
(apply-funk 4)

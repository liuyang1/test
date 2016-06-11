#lang racket

;;; struct
; STRUCT build a struct
; mutable make it have set-NAME-PROPERTY! func
(struct point (x y) #:mutable #:transparent)

(define (on-point p1 p2)
  (set-point-x! p1 0)
  (set-point-y! p2 0)
  (point (+ (point-x p1) (point-x p2))
         (+ (point-y p1) (point-y p2))))

(define p1 (point 2 3))
(define p2 (point 5 7))
(define p3 (on-point p1 p2))
p1
p2
p3

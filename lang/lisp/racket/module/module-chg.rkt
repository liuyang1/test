#lang racket

(provide (all-defined-out))

(define name "abc")

(define (show-name) (displayln name))

(define (set-name! s) (set! name s))

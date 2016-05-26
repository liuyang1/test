#lang racket
; This is like a invidual module file
(module m racket
        (provide color)
        (define color "blue"))
(module n racket
        (provide size)
        (define size 17))
(require 'm 'n)
(list color size)

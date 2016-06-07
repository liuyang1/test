#lang racket

(define *x* (mcons 1 2))

*x*

(mcar *x*)

(mpair? *x*)

(set-mcar! *x* 3)

*x*

(mcar *x*)

(mpair? *x*)

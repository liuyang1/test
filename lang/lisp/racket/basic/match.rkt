#lang racket

(define/match (fact n)
              [(0) 1]
              [(n) (* n (fact (sub1 n)))])

(fact 5)

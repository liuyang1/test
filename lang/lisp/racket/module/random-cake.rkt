#lang racket
; add thie line, forbidden any output when run with `racket -f [filename]`
; but with `radcket [filename]` is ok
(require "cake.rkt")

(print-cake (random 30))

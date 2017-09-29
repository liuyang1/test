#lang racket
;;; ref: https://learnxinyminutes.com/docs/racket/

;; You can have multi-variadic functions too, using `case-lambda'
(define hello3
  (case-lambda
    [() "Hello World"]
    [(name) (string-append "Hello " name)]))
(hello3) ; => "Hello World"
(hello3 "Jake") ; => "Hello Jake"
;; ... or specify optional arguments with a default value expression
(define (hello4 [name "World"])
  (string-append "Hello " name))

;; Functions can pack extra arguments up in a list
;; varidiac arguments, use apply to expand it
(define (count-args . args)
  (format "You passed ~a args: ~a" (length args) args))
(count-args 1 2 3) ; => "You passed 3 args: (1 2 3)"
(define (sum-args . args)
  (let ((v (apply max args)))
   (format "apply (max ~a) get sum ~a" args v)))
(sum-args 1 2 3)
;; ... or with the unsugared `lambda' form:
(define count-args2
  (lambda args
    (format "You passed ~a args: ~a" (length args) args)))
(count-args2 1 2 3)


;; You can mix regular and packed arguments
(define (hello-count name . args)
  (format "Hello ~a, you passed ~a extra args" name (length args)))
(hello-count "Finn" 1 2 3)
; => "Hello Finn, you passed 3 extra args"
;; ... unsugared:
(define hello-count2
  (lambda (name . args)
    (format "Hello ~a, you passed ~a extra args" name (length args))))

;; And with keywords
(define (hello-k #:name [name "World"] #:greeting [g "Hello"] . args)
  (format "~a ~a, ~a extra args" g name (length args)))
(hello-k)                 ; => "Hello World, 0 extra args"
(hello-k 1 2 3)           ; => "Hello World, 3 extra args"
(hello-k #:greeting "Hi") ; => "Hi World, 0 extra args"
(hello-k #:name "Finn" #:greeting "Hey") ; => "Hey Finn, 0 extra args"
(hello-k 1 2 3 #:greeting "Hi" #:name "Finn" 4 5 6)
; => "Hi Finn, 6 extra args"

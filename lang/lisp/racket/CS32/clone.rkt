#lang racket

;;; clone.rkt

;;; The following functions are based on discussions in CS 32 on Wednesday, October 13, 2004.
;;; When asked "What is cons good for?" we constructed the following function called clone that
;;; creates a copy of a list of atoms.

(define (clone l)
  (cond ((null? l) '())
        (else (cons (car l) (clone (cdr l))))))

;;; As a test, we can evaluate the following
(define lat '(apple peach pear plum))

;;; and then evaluate
(clone lat)

;;; No surprise. clone does indeed create a copy, by first breaking the list
;;; apart, using car and cdr, and then reassembling back together using cons.
;;; (So that's what cons is good for.) But perhaps this is not very convincing
;;; because the same result is obtained by simply evaluating
lat

;;; So, in class, there was just enough time to demonstrate one more function.
;;; Let's call it anteater

(define (anteater s)
  (cond ((null? s) '())
        ((eq? (car s) 'ant) (anteater (cdr s))) ;; <- this line is new
        (else (cons (car s) (anteater (cdr s))))))

;;; This function is essentially the same as clone, except that every occurence
;;; of "clone" has been replaced by "anteater" and one new condition has been
;;; inserted in the cond. Now, evaluate the following

(define bugs '(bee wasp ant spider moth fly ant termite ant gnat snail))

bugs

(anteater bugs)
;;; Yum!

;;; Note that all works well, as bugs is a list of atoms. However if we were to
;;; define a more general s-expression, one with deeper levels, like

(define morebugs '((bee moth ant) spider (termite ant (fly (ant (wasp)))) ant (gnat) snail))

;;; and then were to evaluate

(anteater morebugs)

;;; our function is only able to swallow one ant: the one at the top level. Can we design a
;;; better anteater? Of course we can. But let's start by making a better version of clone,
;;; one that works with an arbitrary s-expression, e.g., symbols, lists of atoms, and lists
;;; of lists.

;;; As atoms usually need special treatment (you can't take the car or cdr of an atom), we
;;; shall adopt the predicate atom? from "The Little Schemer",

(define (atom? x)
    (and (not (pair? x)) (not (null? x))))

;;; Now we redefine clone as
(define (clone2 s)
  (cond ((null? s) '())
        ((atom? s) s)
        (else (cons (clone2 (car s)) (clone2 (cdr s))))))


;;; Test
(clone2 bugs)
(clone2 morebugs)


;;; Now we will define a better anteater by changing "clone" to anteater and adding a new item in the cond:
(define (anteater2 s)
  (cond ((null? s) '())
        ((eq? s 'ant) "Yum!") ;; <- only this line is really new!
        ((atom? s) s)
        (else (cons (anteater2 (car s)) (anteater2 (cdr s))))))

;;; And now its dinner time:

(anteater2 morebugs)

;;; Here is a further generalization that defines a function called subst that replaces every occurence of the symbol
;;; defined in argument 1 (from) in the s-expression in argument 3 (s) by the s-expression in argument 2 (to).

(define (subst from to s)
  (cond ((null? s) '())
        ((eq? s from) to) ;; <- only this line is new!
        ((atom? s) s)
        (else (cons (subst from to (car s))
                    (subst from to (cdr s))))))

;;; Using subst, we can redefine anteater and efficiently create some other preditors as well:

;;; the anteater eats only ants.
(define (anteater3 s)
  (subst 'ant "Yum!" s))

;;; The frog only eats flying insects, and doesn't like getting stung.
(define (frog s)
  (subst 'fly "Yum!"
         (subst 'bee "Ouch!"
                (subst 'wasp "YeeOW!!"
                       (subst 'gnat "burp" s)))))


;;; And now we test all three.
(anteater3 morebugs)
(frog morebugs)

(defvar *expr0* (list (list 2 3) (list 4 5)))
(defvar *expr1* (list `(list 2 3) `(list 4 5)))
(defvar *expr2* `(list (list 2 3) (list 4 5)))

*expr0*
*expr1*
*expr2*

; cannot run
(eval *expr0*)
; cannot run
(eval *expr1*)
(eval *expr2*)

; cannot run as first prarm is callable

(defvar *EXPR* (list (random 10) (random 10)))

; *EXPR* random generated when define
*EXPR*
*EXPR*

(defun exprGen () (list (random 10) (random 10)))

; exprGen is func, and run everytime, so we could get random result every time
(exprGen)
(exprGen)

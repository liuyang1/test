(defun random-elt (ls) (nth (random (length ls)) ls))

(random-elt '(11 22 33 44))

; clouser version
(defun random-elt-clj (ls) (lambda () (nth (random (length ls)) ls)))
(defvar random-ls (random-elt-clj '(11 22 33 44)))

; FUNCALL
(funcall random-ls)
(funcall random-ls)
(funcall random-ls)

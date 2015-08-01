(defun dice () (+ 1 (random 6)))
(defun two-dice () (list (dice) (dice)))

; we need call random serveral times to get "real" random number
; (random 60)
; (random 60)
(two-dice)

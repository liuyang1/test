; ugly but fast version
(defun swap-two (ls)
  (cons (first (rest ls)) (cons (first ls) (rest (rest ls)))))

; not so but still ugly
(defun swap-two-1 (ls)
  (cons (nth 1 ls) (cons (first ls) (rest (rest ls)))))

(swap-two '(9 8 7 6))
(swap-two-1 '(9 8 7 6))

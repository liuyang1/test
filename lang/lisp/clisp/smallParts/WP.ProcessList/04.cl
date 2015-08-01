(defun max-list (ls)
  (if (null (rest ls))
    (first ls)
    (max (first ls) (max-list (rest ls)))))

(max-list '(11 13 17 19 2 3 5 7))

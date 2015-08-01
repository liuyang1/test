(defun dupli (ls)
  (if (null ls) '()
   (cons (first ls) (cons (first ls) (dupli (rest ls))))))

(dupli '(a b c d e))

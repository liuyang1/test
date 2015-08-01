(defun interlace (ls0 ls1)
  (cond ((null ls0) ls1)
        ((null ls1) ls0)
        (t (cons (first ls0) (cons (first ls1) (interlace (rest ls0) (rest ls1)))))))

(interlace '(a b c) '(d e f))

; clear but no efficent version
(defun last-elt (ls) (nth (- (length ls) 1) ls))
; rec version
(defun last-elt-1 (ls) (if (null (rest ls)) (first ls)
                         (last-elt (rest ls))))

(last-elt '(1 2 3 4))
(last-elt-1 '(1 2 3 4))

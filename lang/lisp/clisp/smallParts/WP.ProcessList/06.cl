(defun compress-h (ls lasti)
  (cond
    ((null ls) (cons lasti '()))
    ((equal (first ls) lasti) (compress-h (rest ls) lasti))
    (t (cons lasti (compress-h (rest ls) (first ls))))))
(defun compress (ls) (compress-h (rest ls) (first ls)))


(compress '(a a a a b c c a a d e e e e))

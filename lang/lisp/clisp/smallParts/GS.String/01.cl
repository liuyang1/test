(defun midverse (w)
  (concatenate 'string
               (subseq w 0 1)
               (reverse (subseq w 1 (- (length w) 1)))
               (subseq w (- (length w) 1))))

(defvar *str* "retinues")
(midverse *str*)
(string= *str* (midverse (midverse *str*)))

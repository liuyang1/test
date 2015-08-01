(defun rotate (str i)
  (concatenate 'string
               (subseq str i)
               (subseq str 0 i)))

(rotate "mousetrap" 5)

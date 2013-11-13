(defun max2 (a b)
  (if (> a b) a b)
  )

(defun max3 (a b c)
  (max2 (+ a b) (max2 (+ b c) (+ c a)))
  )

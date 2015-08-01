(defun pascal (n r)
  (cond ((or (= n 1) (= n r)) 1)
        (t (+ (pascal (- n 1) (- r 1))
              (pascal n (- r 1))))))

(pascal 3 5)

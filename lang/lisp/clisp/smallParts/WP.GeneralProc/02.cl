(defun combine (fn xs)
  (if (= (length xs) 1) (car xs)
    (funcall fn (car xs) (combine fn (cdr xs)))))

(combine '+ '(1 2 3 4))
(combine '* '(1 2 3 4))

(defun numberpair? (ls)
  (and (= 2 (length ls))
       (numberp (first ls))
       (numberp (first (rest ls)))))

(numberpair? '(45 67))
(numberpair? '("cat" "dog"))
(numberpair? '("cat" "dog" 42))

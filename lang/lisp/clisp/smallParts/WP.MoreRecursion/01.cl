(load "tree.cl")

(defun count-tree (tr)
  (if (null tr) 0
    (let ((left (cadr tr))
          (right (caddr tr)))
      (+ 1 (count-tree left) (count-tree right)))))

(count-tree *tree*)

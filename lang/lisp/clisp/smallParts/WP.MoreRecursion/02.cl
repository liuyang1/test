(load "tree.cl")

(defun find-tree (tr elm)
  (if (null tr) nil
    (or (string= elm (car tr))
        (find-tree (cadr tr) elm)
        (find-tree (caddr tr) elm))))

(find-tree *tree* "n")
(find-tree *tree* "gone")
(find-tree *tree* "tins")

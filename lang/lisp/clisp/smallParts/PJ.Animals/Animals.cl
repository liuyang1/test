; basic prompt function
(defun println (s) (format t "~a~%" s))

(defun prompt-string (s)
  (progn (println s) (format t "> ") (read-line)))

(defun get-yes-no ()
  (progn (format t "Yes or No?~%> ")
         (let ((in (read-line)))
           (if (or (string= in "yes") (string= in "no"))
             in
             (get-yes-no)))))
(defun prompt-yesno (s)
  (progn (println s) (string= "yes" (get-yes-no))))

(defparameter animal-tree '("Does it live in the sea?"
                            "a dolphin"
                            ("Does it have 4 legs"
                             "a horse"
                             ("Can it fly?" "a raven" "a human"))))


; ask question, and built a new tree
(defun learn-animal (animal new-animal)
  (let ((question
          (prompt-string (format t "Give me a yes/no question to distinguish between ~a and ~a?"
                                 new-animal animal))))
    (if (prompt-yesno (format t "What could the answer be for ~a?~%" new-animal))
      (list question animal new-animal)
      (list question new-animal animal))))

; guess an animal, if no, then learn-animal
(defun make-guess (animal)
  (if (prompt-yesno (format nil "Is it ~a?" animal))
    (println "Ho ho!")
    (learn-animal animal (prompt-string "What were you thinkding of?"))))

; ask-question base on tree
(defun ask-question (tree)
  (if (prompt-yesno (first tree))
    (list (first tree) (animals (second tree)) (third tree))
    (list (first tree) (second tree) (animals (third tree)))))

; ask-question when is tree, else directly guess
(defun animals (tree)
  (if (listp tree) (ask-question tree) (make-guess tree)))

(defun play-animals ()
  (when (probe-file "tree")
    (with-open-file (stream "tree" :direction :input)
      (setq animal-tree (read stream))))
  (setq animal-tree (animals animal-tree))
  (with-open-file (stream "tree" :direction :output :if-exists :supersede)
    (write animal-tree :stream stream)))
; (make-guess "cat")
; (learn-animal "a cat" "a dog")
; (ask-question animal-tree)

(play-animals)

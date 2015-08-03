(defparameter wordlist-fn "words.lisp")
(defparameter wordlist
  (with-open-file (stream wordlist-fn :direction :input)
    (read stream)))

(defparameter dict (make-hash-table))

(defun sorted (wd) (sort (copy-seq wd) 'char<))

(defun add-word (wd)
  (let* ((swd (sorted wd)))
    (setf (gethash swd dict) (cons wd (gethash swd dict)))))

; clisp will stack overflow on this func
(defun add-words-to-dict (ws)
  (if (null ws) nil
    (let ((wd (first ws)))
      (add-word wd)
      (add-words-to-dict (rest ws)))))

(add-words-to-dict wordlist)

(defun find-anagram (wd)
  (gethash (sorted wd) dict))


(defun prompt-string (s)
  (progn (print s) (format t "> ") (read-line)))

(defun anagram ()
  (print (find-anagram (prompt-string "Find agaram:"))))

(anagram)

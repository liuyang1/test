(defun hello ()
  (write-line "What's your name?")
  (let ((name (read-line)))
    (format t "Hello, ~A.~%" name)))

(defun print-for (from to)
  (if (> from to) nil
    (progn (format t "~a^2 = ~a~%" from (* from from))
           (print-for (+ 1 from) to))))

(print-for 4 7)

(defun repeat-for (from to fn)
  (if (> from to) nil
    (progn (funcall fn from)
           (repeat-for (+ 1 from) to fn))))

(repeat-for 4 7 'print)

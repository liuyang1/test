; Practical Common lisp chapter 30
; HTML generate library,Parse Module
; auth: liuyang1
; mtime: 2013-01-06 18:29:39 

(defvar *html-output* *standard-output*)

(defun emit-html (html)
  "An interpreter for the literal HTML language"
  (write-sequence html *html-output*))

(defmacro html (html)
  "A compiler for the literal HTML language"
  `(write-sequence ,html *html-output*))

(defun self-evaluating-p (form)
  (and (atom form) (if (symbolp form) (keywordp form) t)))

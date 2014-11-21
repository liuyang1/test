; 定义宏，不是函数，宏本质是一种代码的替换
(defmacro as (tag content)
  `(format t "<~(~A~)>~A</~(~A~)>"
           ',tag ,content ',tag))
; 这里定义的时候使用了很多特殊符号，这里一一解释下。
; 第一个反引号，表示阻止求值，
;       但是内部中以逗号开始的部分是可以进行求值的。
; 单引号，用于quote，避免求值
; 逗号，用于反引号组合使用
;
; 就此解释：逗号content，表示后面的内容会进行计算之后进行替换
; ',tag表示不会进行求值直接计算
; 'tag则直接只输出“tag”
; ,tag则会尝试对tag进行计算,这种方式如果tag是一个字符串的话，是可以的
; 但是当tag不是字符串标识的会就会尝试求值center，进而出现center没有对应值的错误

(defmacro with (tag &rest body)
  `(progn
     (format t "~&<~(~A~)>~%" ',tag)
     ,@body
     (format t "~&</~(~A~)>~%" ',tag)))

(defmacro brs (&optional (n 1))
  (fresh-line)
  (dotimes (i n)
    (princ "<br>"))
  (terpri))


; test code
; (as "center" (+ 1 2))
; (as center (+ 1 2))
;(with center
;      (princ "the unbalanced parenthesis"))
;(brs)
;(brs 2)

(defun html-file (base)
  (format nil "~(~A~).html" base))

(defmacro page (name title &rest body)
  (let ((ti (gensym)))
    `(with-open-file (*standard-output*
                       (html-file ,name)
                       :direction :output
                       :if-exists :supersede)
       (let ((,ti ,title))
         (as title ,ti)
         (with center
               (as h2 (string-upcase ,ti)))
         (brs 3)
         ,@body))))
; (page 'paren "Unbal" (princ "std"))

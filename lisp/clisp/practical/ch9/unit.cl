; Practical Common lisp chapter 9
; Practice: build Unit Test framework
; auth: liuyang1
; mtime: 2013-01-06 16:15:14

(defun report-result (ret form)
	 (format t "~:[FAIL~;pass~] ... ~a~%" ret form))

(defmacro chk0 (form)
  `(report-result ,form ',form))

(defun test-+0 ()
  (chk0 (= (+ 1 2) 3))
  (chk0 (= (+ 1 2 3) 6)))
; 存在问题
; 还有代码冗余,需要多次输入chk0

(defmacro chk1 (&body forms)
  `(progn
	 ,@(loop for f in forms collect `(report-result ,f ',f))))

(defun test-+1 ()
  (chk1
	(= (+ 1 2) 3)
	(= (+ 1 2 3) 6)
	(= (- 1 2) 1)
	))
; 存在问题
; 不论chk1内部的检查是否为T/NIL,返回值始终为NIL
; (不可以使用AND,因为存在逻辑计算的短路)

; using this with-gensyms-ext instead of with-gensyms 
; sth different
(defmacro with-gensyms-ext ((&rest names) &body body)
  `(let ,(loop for n in names collect `(,n (gensym)))
	 ,@body))

; 这个宏是引入变量result(使用with-gensyms创建唯一的名字),然后展开循环赋值,如果 ,f 表达式为假,则赋值为nil,否则保持为t
(defmacro combine-results (&body forms)
  (with-gensyms-ext (result)
				`(let ((,result T))
				   ,@(loop for f in forms collect `(unless ,f (setf ,result NIL)))
				   ,result)))

; replace progn by combine-results MACRO
(defmacro chk2 (&body forms)
  `(combine-results
	 ,@(loop for f in forms collect `(report-result1 ,f ',f))))

; new version of report-result1
; return result value
(defun report-result1 (result form)
	 (format t "~:[FAIL~;pass~] ... ~a~%" result form)
	 result)

(defun test-+2 ()
  (chk2
	(= (+ 1 2) 3)
	(= (+ 1 2 3) 6)
	))

; Practical Common lisp chapter 9
; mtime: 2013-01-06 17:55:18 
; auth: liuyang1 

; final version

(defvar *test-name* nil)

(defmacro deftest (name param &body body)
  "Define a test function.Within a test function we can call other test functions or use 'check' to run individual test cases."
  `(defun ,name ,param
	 (let ((*test-name* (append *test-name* (list ',name))))
	   ,@body)))

(defmacro check (&body forms)
  "Run each expression in 'forms' as a test case."
  `(combine-results
	 ,@(loop for f in forms collect `(report-result ,f ',f))))

(defmacro with-gensyms-ext ((&rest names) &body body)
  "generate symbol for unique name as tmp var."
  `(let ,(loop for n in names collect `(,n (gensym)))
	 ,@body))

(defmacro combine-results (&body forms)
  "Combine the results (as boolean) of evaluating 'forms' in order."
  (with-gensyms-ext (s-ret)
					`(let ((,s-ret t))
					   ,@(loop for f in forms collect `(unless ,f (setf ,s-ret nil)))
					   ,s-ret)))

(defun report-result (result form)
  "Report the results of a single test case. called by 'check'."
  (format t "~:[FAIL~;pass~] ... ~a: ~a~%" result *test-name* form)
  result)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; a use case

(deftest test-arith ()
		 (combine-results
		   (test-+)
		   (test-*)))

(deftest test-+ ()
		 (check
		   (= (+ 1 2) 3)
		   ))

(deftest test-* ()
		 (check
		   (= (* 1 2) 2)
		   ))

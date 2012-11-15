(in-package :cl-user)

(defpackage :com.binary-data
 ; (:use :common-lisp :com.gigamokeys.macro-utilities)
  (:export :define-binary-class
		   :define-tagged-binary-class
		   :define-binary-type
		   :read-value
		   :wirte-value
		   :*in-progress-objects*
		   :parent-of-type
		   :current-binary-object
		   :+null+))


(defun as-keyword (sym) (intern (string sym) :keyword))
(defun slot->defclass-slot (spec)
  (let ((name (first spec)))
	`(,name	:initarg	,(as-keyword name)	:accessor	,name)))

; for test slot->defclass-slot method
(print (slot->defclass-slot '(major-version u1)))

; first edtion, replace by better method
; (defmacro define-binary-class (name slots)
;   `(defclass ,name ()
; 	 ,(mapcar #'slot->defclass-slot slots)))

; for test define-binary-class method
(print
(macroexpand-1 '(define-binary-class id3-tag
									 ((identifier		(iso-8859-1-string :length 3))
									  (major-version	u1)
									  (revison			u1)
									  (flags			u1)
									  (size				id3-tag-size)
									  (frames			(id3-frames	:tag-size size)))))
)
; 由此,可以看出lisp的宏的力量的强大,用来解析编码,可以说编码效率非常高.
; 基础工作做好以后,只需要进行一次定义,就可以整体完成所有的解析过程

; (defgeneric read-value (type stream &key)
; 			(:documention "Read a value of the given type from the stream."))
; (defmethod read-value ((type (eql 'iso-8859-1-string)) in &key lenghth)...);???
; (defmethod read-value ((type (eql 'u1)) in &key)...);TODO

(defun slot->read-value (spec stream)
  (destructuring-bind (name (type &rest args)) (normalize-slot-spec spec)
					   `(setf ,name (read-value ',type ,stream ,@args))))
	(defun normalize-slot-spec (spec)
	  (list (first spec) (mklist (second spec))))
	(defun mklist (x) (if (listp x) x (list x)))

(print
  (slot->read-value '(major-version u1) 'stream))
(print
  (slot->read-value '(identifier (iso-8859-1-string :length 3)) 'stream))

(defmacro define-binary-class (name slots)
  (with-gensym (typevar objectvar streamvar)
			   `(progn
				  (defclass ,name ()
					,(mapcar #'slot->defclass-slot slots))

				  (defmethod read-value ((,typevar (eql ',name)) ,streamvar &key)
					(let ((,objectvar (make-instance ',name)))
					  (with-slots ,(mapcar #'first slots) ,objectvar ,@(mapcar #'(lambda (x) (slot->read-value x streamvar)) slots))
					  ,objectvar)))))

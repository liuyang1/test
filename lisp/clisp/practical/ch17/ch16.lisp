; Practical Common lisp chapter 17
; review Object-Oriented

(defvar *global-account-num* 0)

(defclass bank-account ()
  ((account-number
     :initform (incf *global-account-num*)
     )
   (customer-name
     :initarg :name
     :initform (error "Must supply a customer name.")
     )
   (balance
     :initarg :balance
     :initform 0
     )
   )
  )

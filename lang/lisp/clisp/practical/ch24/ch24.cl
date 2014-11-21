; ldb func
; var-in: byte descriptor and data
;
; !!byte descriptor!!
; (byte length-index begin-index)

; read two byte unsigned intger
(defun read-u2 (in)
  (let ((u2 0))
	(setf (ldb (byte 8 8) u2) (read-byte in))
	(setf (ldb (byte 8 0) u2) (read-byte in))
	u2))

; write two bytes unsigned intger
(defun write-u2 (out value)
  (write-byte (ldb (byte 8 8) value) out)
  (write-byte (ldb (byte 8 0) value) out))

; read string which end with '\0'
(defconstant +null+ (code-char 0))
(defun read-null-terminated-ascii (in)
  (with-ouput-to-string (s)
						(loop for char = (code-char (read-byte in))
							  until (char= char +null+) do (write-char char s))))

; write string which end with '\0'
(defun write-null-terminated-ascii (string out)
  (loop for char across string
		do (write-byte (char-code char) out))
  (write-byte (char-code +null+) out))


; define class id3-tag
(defclass id3-tag ()
  ((identifier		:initarg :identifier	:accessor identifier)
   (major-version	:initarg :major-version :accessor major-version)
   (revison			:initarg :revison		:accessor revison)
   (flags			:initarg :flags			:accessor flags)
   (size			:initarg :size			:accessor size)
   (frames			:initarg :frames		:accessor frames)))

; read id3-tag
(defun read-id3-tag (in)
  (let ((tag (make-instance 'id3-tag)))
	(with-slots (identifier major-version revison flags size frames) tag
	  (setf identifier		(read-iso-8859-1-string in :length 3))
	  (setf major-version	(read-u1 in))
	  (setf revison			(read-u1 in))
	  (setf flags			(read-u1 in))
	  (setf size			(read-id3-encoded-size in))
	  (setf frames			(read-id3-frames in :tag-size size)))
	tag))
; ----
; this idea is just like C style
; too many code



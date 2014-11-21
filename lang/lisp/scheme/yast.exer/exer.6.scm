(define (quadEqu a b c)
  (if (zero? a)
	'error
	(let ((d (- (* b b) (* 4 a c))))
	  (if (negative? d)
		'()
		(let ((e (/ b a -2)))
		  (if (zero? d)
			(list e)
			(let ((f (/ (sqrt d) a 2)))
			  (list (+ e f) (- e f)) )))))))

;exer 6.1
(define (throw v theta)
  (let ((ang (* (/ theta 180) pi)))
	(let ((vx (* v (sin ang))) (vy (* v (cos ang))))
	  (* vx (/ (* 2 vy) 9.8))
	  )))

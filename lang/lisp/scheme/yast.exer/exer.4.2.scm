;http://www.shido.info/lisp/scheme4_e.html
;exercise 2
;exer1 define a function that convert angle from degree to radian
(define pi (* 4 (atan 1.0)))
(define (deg2rad val) (* pi (/ val 180)))
;exer2 define a func that calc dist of moving at a constant speed in t second
(define (dist t vx) (* vx t))
;exer3 calc a duration till the obj reached the ground that is launched with vy.
;t=2*vy/g
(define (dura vy) (/ (* 2 vy) 9.8))
;exer4 flying dist
(define (throw v theta)
		(dist (dura (* v (sin (deg2rad theta)))) (* v (cos (deg2rad theta))))
		)
;exer5 
;(throw 40 30)

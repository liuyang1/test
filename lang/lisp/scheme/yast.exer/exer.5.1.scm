;exer 5.1.1 abs of real num
(define (myabs val) (if (> val 0) val (* -1 val)))
;exer 5.1.2 reciprocal of real num
(define (recip val) (if (= val 0) #f (/ 1 val)))
;exer 5.1.3 convert int to char
(define (itoa val) (if (and (< val 33) (> val 126)) #f (integer->char val)))

;exer 5.2.1
;prod of three num
(define (prod3p a b c) (if (and (> a 0) (> b 0)  (> c 0)) (* a b c) #f))
;exer 5.2.2
(define (prod3n a b c) (if (or (< a 0) (< b 0)  (< c 0)) (* a b c) #f))

;exer 5.3.1
(define (grade score)
  (cond
	((>= score 80) "A")
	((and (>= score 60) (<= score 79)) "B")
	((and (>= score 40) (<= score 59)) "C")
	(else "D")))

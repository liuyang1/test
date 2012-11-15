;exer 8.1.1 twice
(define (twice lst) (map (lambda (x) (+ x x)) lst))
;exer 8.1.2 sub of two list
(define (sub ls0 ls1) (map (lambda (x y) (- x y)) ls0 ls1))

;exer 8.2.1 filtering out even num in lst
(define (f_even lst) (keep-matching-items lst even?))
;exer 8.2.2 filtering out num that are not 10<=x<=100
(define (f_range lst) (delete-matching-items lst (lambda (x) (and (>= x 10) (<= x 100)))))

;exer 8.3
(define (func lst) (sqrt (reduce + 0 (map (lambda (x) (* x x)) lst))))

;exer 8.4
;sort by mag of sin(x) in asceding order
(define (sortsin lst) (sort lst (lambda (x y) (< (sin x) (sin y)))))
;exer 8.4.2
;sort by length
(define (len lst) (if (null? lst) 0 (+ 1 (len (cdr lst)))))
(define (sortlen lst) (sort lst (lambda (x y) (> (len x) (len y)))))

;exer 8.5
;using APPLY to impl exer 8.3
(define (func_apply lst)
  (sqrt (apply + (map (lambda (x) (* x x)) lst))))

;define member & member-if
(define (mymember proc obj ls)
  (cond
	((null? ls) #f)
	((proc obj (car ls)) ls)
	(else (mymember proc obj (cdr ls)))))

; define keep-matching_items of your own
(define (mykeep_h f ls0 ls1)
  (if (null? ls0) ls1
    (if (f (car ls0)) (mykeep_h f (cdr ls0) (cons (car ls0) ls1))
	  (mykeep_h f (cdr ls0) ls1))))
(define (mykeep ls f)
  (reverse (mykeep_h f ls ())))

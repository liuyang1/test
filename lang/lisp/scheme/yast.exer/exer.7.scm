;exer 7.1.1
(define (mylen lst)
  (if (null? lst) 0 (+ 1 (mylen (cdr lst))))
  )
;exer 7.1.2
(define (mysum lst)
  (if (null? lst) 0 (+ (car lst) (mysum (cdr lst)))))
;exer 7.1.3
(define (sub lst x)
  (if (null? lst) '(); !!!must consider this!!!
	(cons (- (car lst) x) (sub (cdr lst) x))))
;exer 7.1.4
(define (find ls x)
  (if (= (car ls) x) 1 (+ 1 (find (cdr ls) x))))

;exer 7.2.1
(define (rev ls)
  (revt ls ()))
(define (revt ls t)
  (if (null? ls) t (revt (cdr ls) (cons (car ls) t))))

;exer 7.2.2
(define (sum2 lst)
  (sum2t lst 0))
(define (sum2t lst t)
  (if (null? lst) t (sum2t (cdr lst) (+ t (car lst)))))
;expr 7.2.3
(define (str2num str)
  (let ((lst (string->list str)))
	(lst2num lst 0)))
(define (lst2num lst t)
  (if (null? lst) t (lst2num (cdr lst) (+ (* t 10) (- (char->integer (car lst)) 48)))))

;exer 7.3.1.1
(define (sub_namelet lst x)
  (let loop((ls0 lst) (ls1 ()))
	(if (null? ls0)
	  ls1
	  (loop (cdr ls0) (cons (- (car ls0) x) ls1)))))
;exer 7.3.1.2
(define (find_namelet lst x)
  (let loop((ls0 lst) (index 0))
	(if (null? ls0)
	  #f
	  (if (= (car ls0) x) index (loop (cdr ls0) (+ 1 index))))))
;exer 7.3.2.1
(define (rev_namelet lst)
  (let loop((ls0 lst) (ls1 ()))
	(if (null? ls0)
	  ls1
	  (loop (cdr ls0) (cons (car ls0) ls1)))))
;exer 7.3.2.2 using namelet impl sun
(define (sum_namelet lst)
  (let loop((ls0 lst) (sum 0))
	(if (null? ls0) sum (loop (cdr ls0) (+ (car ls0) sum)))))
;exer 7.3.2.3 using namelet impl str2num
(define (str2num_namelet str)
  (let ((lst (string->list str)))
	(let loop((ls0 lst) (num 0))
	  (if (null? ls0) num (loop (cdr ls0) (+ (- (char->integer (car ls0)) 48) (* 10 num)))))))
;exer 7.3.3 range func,so beautiful!
(define (range n)
  (let ((inc (if (negative? n) 1 -1)))
	(let loop((lst ()) (num n))
	  (let ((ni (+ inc num)))
	    (if (zero? num) lst
		  (loop (cons ni lst) ni))))))

(define (fact_letrec n)
  (letrec ((iter (lambda (n1 p) 
				   (if (= n1 1)
					 p 
					 (let ((m (- n1 1))) 
					   (iter m (* p m)))))))
	(iter n n)))
;exer 7.4.1
;(define (rev_letrec lst)
;  (letrec ((iter (lambda (

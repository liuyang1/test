; need this func,which introduced in [The Little Schemer]
(define atom?
  (lambda (x)
    (and (not (pair? x)) (not (null? x)))
    )
  )

(define isNULL
  (lambda (lat)
    (cond
      ((null? lat) (print "null"))
      (else (print "NOT null"))
      )
    )
  )
; 这个是错误的?验证总是不对
; XXX
(define member?
  (lambda (a lat)
    (cond
      ((null? lat) #f)
      (else (or (eq? a (car lat)))
            (member? a (cdr lat))
            )
      )
    )
  )

(define two-in-a-row?
  (lambda (lat)
    (cond
      ((null? lat) #f)
      (else
        (or (is-first? (car lat) (cdr lat))
            (two-in-a-row? (cdr lat)))))))

(define is-first?
  (lambda (a lat)
    (cond
      ((null? lat) #f)
      (else (eq? a (car lat)))
      )
    )
  )

(define two-in-a-row-b?
  (lambda (preceding lat)
    (cond
      ((null? lat) #f)
      (else (or (eq? preceding (car lat))
                (two-in-a-row-b? (car lat) (cdr lat))
                ))
      )
    )
  )

(define two-in-a-row-b-ex?
  (lambda (lat)
    (cond
      ((null? lat) #f)
      (else
        (two-in-a-row-b? (car lat) (cdr lat))
        )
      )
    )
  )
; ----
(define sum-of-prefixes
  (lambda (tup)
    (cond
      ((null? tup) '())
      (else ..
            (sum-of-prefixes (cdr tup))
            ..
            )
      )
    )
  )
; ----
; use Additonal arguments when a func needs to know what other args to the func have been like so far
; using ARGs instead of Var
(define sum-of-prefixes-b
  (lambda (sum tup)
    (cond
      ((null? tup) '())
      (else (cons (+ sum (car tup))
                  (sum-of-prefixes-b (+ sum (car tup)) (cdr tup))
                  ))
      )
    )
  )

(define sum-of-prefixes-b-ex
  (lambda (tup)
    (sum-of-prefixes-b 0 tup)
    )
  )
; ----
; page 13

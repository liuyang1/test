(define (flatmap proc seq)
  (accumulate append '() (map proc seq)))

(define (range low high)
  (if (> low high) '()
    (cons low (range (+ low 1) high))))

(define (accumulate-iter op init seq)
  (define (helper seq ret)
    (if (null? seq) ret
      (helper (cdr seq) (op (car seq) ret))))
  (helper seq init))

(define (accumulate-rec op initial sequence)
  (if (null? sequence) initial
    (op (car sequence)
        (accumulate-rec op initial (cdr sequence)))))

(define accumulate accumulate-iter)

; 判断k行,是否和后面已经排列好的,存在相同的行
(define (same-cols? k positions)
  (cond ((null? positions) #t)
        ((= k (car positions)) #f)
        (else (same-cols? k (cdr positions)))))

; 判断k,是否和后面已经排列好的,存在在对角线位置
(define (transpose? k positions)
  ; nl这里计算k,和后面positions第一项相差的行数
  (define (helper k positions nl)
    (cond ((null? positions) #t)
          ((or (= (+ k nl) (car positions)) (= (- k nl) (car positions))) #f)
          (else (helper k (cdr positions) (+ nl 1)))))
  (helper k positions 1))

; 注意,这里和题目略有区别,k不再是参数了 
; 判断是否不会相互攻击
(define (safe? positions)
    (and (same-cols? (car positions) (cdr positions))
         (transpose? (car positions) (cdr positions))))

(define (queens board-size)
  ; 空局面
  (define empty-board '())
  ; 添加新的位置,直接继续cons就可以了.注意,这里的k参数也是不需要的.
  (define (adjoin-position new-row k rest-of-queens)
    (cons new-row rest-of-queens))
  (define (queen-cols k)
    (if (= k 0) (list empty-board)
      (filter safe?
              (flatmap (lambda (rest-of-queens)
                         (map (lambda (new-row)
                                (adjoin-position new-row k rest-of-queens))
                              (range 1 board-size)
                           ))
                       (queen-cols (- k 1))))))
  (queen-cols board-size))

; 这个算法速度非常快啊,令人惊讶.其实并不快的.
; 这个算法,因为会枚举各种情况,因此需要的内存容量也比较大.
; 对于规模为13的问题,可能就需要几百M内存了.(使用guile)
(define (displayln x) (display x) (newline))
(map displayln (queens 13))

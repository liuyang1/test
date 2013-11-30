(define (left-branch tree) (cadr tree))
(define (right-branch tree) (caddr tree))
(define (entry tree) (car tree))

(define (tree->list-1 tree)
  (if (null? tree)
    '()
    (append (tree->list-1 (left-branch tree))
            (cons (entry tree)
                  (tree->list-1 (right-branch tree))))))

(define (tree->list-2 tree)
  (define (copy-to-list tree res)
    (if (null? tree) res
      (copy-to-list (left-branch tree)
                    (cons (entry tree)
                          (copy-to-list (right-branch tree)
                                        res)))))
  (copy-to-list tree '()))

; 打印一支树
; 按照tree这个命令的样子
(define (disp1 tree)
  ; 打印前缀的最后引导符号
  (define (end)
    (display "`-- "))
  ; 打印前缀
  (define (disPreix prefix)
    (if (= (length prefix) 1)
      (display "`-- ")
    (if (null? prefix) #t
      (if (eq? (car prefix) 'l)
        ; 打印标示层级结构
        (begin (display "|   ") (disPreix (cdr prefix)))
        (begin (display "    ") (disPreix (cdr prefix)))))))
  ; 打印树的所有孩子
  (define (showlist tree prefix)
    (if (null? (cdr tree))
      ; 打印最后一个孩子
      (hlp (car tree) (append prefix ( list 'r)))
      (begin (hlp (car tree) (append prefix (list 'l)))
             (showlist (cdr tree) prefix))))
  ; 打印树
  (define (hlp tree prefix)
    (begin (disPreix prefix)
           (if (null? tree)
             (displayln "end")
             (begin (displayln (entry tree))
                    (showlist (cdr tree) prefix)))))
  (hlp tree '()))


(define *tree* '(52 (555 () ) (189 (42 () (8111 () ())) ())  (234  () (911 () ()))))
(displayln *tree*)
(newline)
(disp1 *tree*)

(displayln (tree->list-1 *tree*))
(displayln (tree->list-2 *tree*))

(define (simph f a b n)
  (define (integral f a b h)
    (define (sum f a b h)
        (if (= a b) (f b)
                (+ (* 2 (f a)) (* 4 (f (+ a h))) (sum f (+ a (* h 2)) b h))))
                (* (/ h 3) (- (sum f a b h) (f a))))
  (integral f a b (/ (- b a) n)))

(define (cube x) (* x x x))

(define (sum term a next b)
  (if (> a b)
    0
    (+ (term a) (sum term (next a) next b))))

(define (integral f a b dx)
  (define (add-dx x) (+ x dx))
  (* (sum f (+ a (/ dx 2)) add-dx b)
     dx))

(define (myinterg f a b n)
  (integral f a b (/ (- b a) n)))

(displayln (myinterg cube 0 1 100))
(displayln (simph cube 0 1 100))

#lang racket
;;; unscramble.rkt
;;; (unsramble str) attempts to find a word in a standard dictionary that matches an anagram of str. If no matching
;;; anagram is found, the function returns #false. Here str is an input character string, e.g., "gif". thus,
;;;
;;; (unscramble "gif") => "fig"
;;;
;;;
;;;;;;;;;;;;;;;;;;;;;;;;;;

;; Set the path for the word dictionary on the current host.
(define *dictionary-path* "web2.txt")

;;; Two sorting algorithms are used: insertion sort for sorting the letters in a word (or anagram), and
;;; quicksort, for sorting the anagram dictionary. Note that insertion sort is optimal for short lists,
;;; where quicksort is better for longer ones.

;;; Insertion Sort:

;; insert1 is the fundamental operation of insertion sort, the kernel of the algorithm (if you will). It
;; inserts the indicated item (argument 3) into its correct position in the sorted list ls (argument 2).
;; The first argument, sort-fn, should be a predicate function of two arguments that returns true if the latter
;; two items are correctly ordered. Thus to create a list of integers in non-decreasing (increasing) order, sort-fn
;; should be replaced by <. (<= can work faster, but at the loss of stability with respect to secondary
;; sorting.) For sorting integers in non-increasing (decreasing) order, select >. Insertion sort for character
;; and string types are shown below.

(define (insert1 sort-fn ls item)
  (cond ((null? ls) (list item))
        ((sort-fn item (car ls)) (cons item ls))
        (else (cons (car ls) (insert1 sort-fn (cdr ls) item)))))

;; insertion-sort-aux applies insert1 repeatedly over the input list in (argument 3) of unsorted records,
;; accumulating the sorted list in out (argument 2). Note on its initial call, out should be an empty list,
;; '().
(define (insertion-sort-aux sort-fn out in)
  (if (null? in)
      out
      (insertion-sort-aux sort-fn (insert1 sort-fn out (car in)) (cdr in))))

;; insert-sort-ints sorts a list of integers in non-decreasing order
;; (insert-sort-ints '(4 2 5 1 9)) => (1 2 4 5 9)
(define (insertion-sort-int ls)
  (insertion-sort-aux < '() ls))

;; insert-sort-str sorts a list of strings in alphabetical (lexicographic) order
;;  (insertion-sort-str '("eel" "ant" "termite" "mouse" "bee" "wasp" "grasshopper")) =>
;;     ("ant" "bee" "eel" "grasshopper" "mouse" "termite" "wasp")
(define (insertion-sort-str ls)
  (insertion-sort-aux string<? '() ls))

;; insert-sort-char sorts a list of characters in alaphabetical order
;; (insertion-sort-char '(#\c #\a #\e)) => (#\a #\c #\e)
(define (insertion-sort-char ls)
  (insertion-sort-aux char<? '() ls))

;; Testing insertion sort:

(define (test-is)
  (printf "Testing insertion sort ...\n")
  (printf "(insertion-sort-int '(40 12 15 38 12 98 13 17 43)) => ")
  (print (insertion-sort-int '(40 12 15 38 12 98 13 17 43)))
  (newline)
  (printf "(insertion-sort-char '(#\\x #\\z #\\a #\\c #\\y #\\b)) => ")
  (print (insertion-sort-char '(#\x #\z #\a #\c #\y #\b)))
  (newline)
  (printf "(insertion-sort-str '(\"eel\" \"ant\" \"termite\" \"mouse\" \"bee\" \"wasp\" \"grasshopper\") => ")
  (print (insertion-sort-str '("eel" "ant" "termite" "mouse" "bee" "wasp" "grasshopper")))
  (newline) (newline))


(test-is)


;;; Quicksort is typically a more efficient sorting algorithm for longer lists of ordered data. As before,
;;; let sort-fn denote a predicate function, such that (sort-fn a b) returns #f if a should not appear
;;; after b. (Because the pivots are taken from the front of the list, we usually desire these to be <
;;; functions for stability.) Two auxillary functions, above and below, split the remaining data around the
;;; pivot.

(define (above sort-fn ls x)
  (cond ((null? ls) '())
        ((sort-fn (car ls) x) (above sort-fn (cdr ls) x))
        (else (cons (car ls) (above sort-fn (cdr ls) x)))))

(define (below sort-fn ls x)
  (cond ((null? ls) '())
        ((sort-fn (car ls) x) (cons (car ls) (below sort-fn (cdr ls) x)))
        (else (below sort-fn (cdr ls) x))))

(define (qsort sort-fn ls)
  (if (null? ls)
     '()
      (append (qsort sort-fn (below sort-fn (cdr ls) (car ls)))
              (list (car ls))
              (qsort sort-fn (above sort-fn (cdr ls) (car ls))))))

(define (qsort-int ls)
  (qsort < ls))

(define (qsort-char ls)
  (qsort char<? ls))

(define (qsort-str ls)
  (qsort string<? ls))


;; Testing quicksort:

(define (test-qs)
  (printf "Testing quicksort ...\n")
  (printf "(qsort-int '(40 12 15 38 12 98 13 17 43)) => ")
  (print (qsort-int '(40 12 15 38 12 98 13 17 43)))
  (newline)
  (printf "(qsort-char '(#\\x #\\z #\\a #\\c #\\y #\\b)) => ")
  (print (qsort-char '(#\x #\z #\a #\c #\y #\b)))
  (newline)
  (printf "(qsort-str '(\"eel\" \"ant\" \"termite\" \"mouse\" \"bee\" \"wasp\" \"grasshopper\") => ")
  (print (qsort-str '("eel" "ant" "termite" "mouse" "bee" "wasp" "grasshopper")))
  (newline) (newline))


(test-qs)


;;; Quicksort2 operates like quicksort but sorts lists of records, where each record is also a list
;;; having the sorting key in the initial position. The remaining content of the record is regarded
;;; as satellite data.

(define (above2 sort-fn ls x)
  (cond ((null? ls) '())
        ((sort-fn (caar ls) (car x)) (above2 sort-fn (cdr ls) x))
        (else (cons (car ls) (above2 sort-fn (cdr ls) x)))))

(define (below2 sort-fn ls x)
  (cond ((null? ls) '())
        ((sort-fn (caar ls) (car x)) (cons (car ls) (below2 sort-fn (cdr ls) x)))
        (else (below2 sort-fn (cdr ls) x))))

(define (qsort2 sort-fn ls)
  (if (null? ls)
     '()
      (append (qsort2 sort-fn (below2 sort-fn (cdr ls) (car ls)))
              (list (car ls))
              (qsort2 sort-fn (above2 sort-fn (cdr ls) (car ls))))))

(define (qsort2-int ls)
  (qsort2 < ls))

(define (qsort2-char ls)
  (qsort2 char<? ls))

(define (qsort2-str ls)
  (qsort2 string<? ls))

;; Testing quicksort:

(define (test-qs2)
  (printf "Testing quicksort2 ...\n")
  (printf "(qsort2-int '((40 a) (12 b) (15 c) (38 d) (12 e) (98 f) (13 g) (17 h) (43 i)) => ")
  (print (qsort2-int '((40 a) (12 b) (15 c) (38 d) (12 e) (98 f) (13 g) (17 h) (43 i))))
  (newline)
  (printf "(qsort2-char '((#\\x 1) (#\\z 2) (#\\a 3) (#\\c 4) (#\\y 5) (#\\b 6)) => ")
  (print (qsort2-char '((#\x 1) (#\z 2) (#\a 3) (#\c 4) (#\y 5) (#\b 6))))
  (newline)
  (printf "(qsort2-str '((\"eel\" 3) (\"ant\" 3) (\"termite\" 7) (\"mouse\" 5) (\"bee\" 3) (\"wasp\" 4) (\"grasshopper\" 11)) => ")
  (print (qsort2-str '(("eel" 3) ("ant" 3) ("termite" 7) ("mouse" 5) ("bee" 3) ("wasp" 4) ("grasshopper" 11))))
  (newline) (newline))


(test-qs2)


;;; Binary search

(define (binary-search-aux v lo hi key)
  (if (<= lo hi)
      (let* ((i (floor (/ (+ lo hi) 2)))
            (x (vector-ref v i)))
        (cond ((< key x) (binary-search-aux v lo (sub1 i) key))
              ((< x key) (binary-search-aux v (add1 i) hi key))
              (else (list i x))))
      #f))

(define (binary-search v key)
  (binary-search-aux v 0 (sub1 (vector-length v)) key))

(define (binary-search2-aux v lo hi key)
  (if (<= lo hi)
      (let* ((i (floor (/ (+ lo hi) 2)))
            (x (car (vector-ref v i))))
        (cond ((string<? key x) (binary-search2-aux v lo (sub1 i) key))
              ((string<? x key) (binary-search2-aux v (add1 i) hi key))
              (else (cdr (vector-ref v i)))))
      #f))


(define (binary-search2 v key)
  (binary-search2-aux v 0 (sub1 (vector-length v)) key))


;;; Input a dictionary

(define (read-dictionary)
  (call-with-input-file *dictionary-path*
    (lambda (ip)
      (let ((dictionary '()))
        (do ([word (read-line ip) (read-line ip)])
          ((eof-object? word))
          (set! dictionary (cons word dictionary)))
        (reverse dictionary)))))

;;; word->key maps one string into another by sorting its letters in alphabetical order, thus
;;; (word->key "play") => "alpy"
(define (word->key str)
  (list->string (insertion-sort-char (string->list (string-downcase str)))))

(define (condense-records ls)
  (cond ((null? ls) ls)
        ((null? (cdr ls)) ls)
        ((string=? (caar ls) (caadr ls)) (condense-records (cons (cons (caar ls) (append (cdar ls) (cdadr ls))) (cddr ls))))
        (else (cons (car ls) (condense-records (cdr ls))))))

(define (make-anagram-table)
  (let ((t (read-dictionary)))
    (list->vector (condense-records (qsort2-str (map (lambda (w) (list (word->key w) w)) t))))))

(define *table* (make-anagram-table))


(define (unscramble w)
  (let ((key (word->key w)))
    (binary-search2 *table* key)))

(define (scramble w)
  (map list->string (remove-duplicates (permutations (string->list w)))))
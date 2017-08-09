#lang racket

;;; dfs.rkt

;;; Depth First Search (also known as Tarry's algorithm).

(define *test-mode* #t)

;;; Here a and b denote two distinct sets, each represented as a list
;;; of items. (set-difference a b) returns the elements contained in
;;; a that are not contained in b. Thus, for example,
;;; (set-difference '(1 2 3 4 5) '(2 4 6)) returns (1 3 5).
;;; Note that it is a bit like the anteater function. It also works for
;;; multisets.

(define (set-difference a b)
  (cond ((null? a) '())
        ((memq (car a) b) (set-difference (cdr a) b))
        (else (cons (car a) (set-difference (cdr a) b)))))

;;; dfs stands for depth-first search. dfs accepts four arguments.
;;;
;;; The first argument, paths, is a list of partial tenative solutions. Since
;;; each partial tenativesolution is represented as a list, paths is a list of lists.
;;; Initially, paths should be a singleton list that contains a singleton list that
;;; represents the intial state, e.g., '((a)) for the hampton court maze. Then as the
;;; function recurs, the paths are extended. Some paths may fork into multiple
;;; paths at junctions, and some will eventually vanish at dead ends.
;;;
;;; The second argument, old-states, is list that represents the set of states that
;;; have been visited. In the context of the maze, this would be the set of nodes
;;; that have been visited during the course of the search. The initial value should
;;; always be an empty list.
;;;
;;; successors represents a successors function as described above, e.g. hc-successors.
;;;
;;; goal? represents a goal predicate as described above, e.g., hc-goal?

(define (dfs paths old-states successors goal?)
  (display paths) (display " ") (display old-states) (newline) ;; optional diagnostics
  (cond ((null? paths) #f)
        ((goal? (caar paths)) (reverse (car paths)))
        (else (dfs (append (map (lambda (x) (cons x (car paths)))
                                (set-difference (successors (caar paths)) old-states))
                           (cdr paths))
                   (cons (caar paths) old-states)
                   successors
                   goal?))))


;;; The inner workings of the dfs function might be clearer if we employ the let
;;; function, which enables us to bind temporarily values to symbols with informative
;;; names

(define (dfs2 paths old-states successors goal?)
  ;; (display paths)
  (if (null? paths)
      #f  ;; indicates that the puzzle cannot be solved.
      (let* ((current-path (car paths))
             (current-state (car current-path)))
        (if (goal? current-state)
            (reverse current-path)
            (let* ((adjacent-states (successors current-state))
                   (new-states (set-difference adjacent-states old-states))
                   (new-branches (map (lambda (x) (cons x current-path)) new-states))
                   (paths (append new-branches (cdr paths)))
                   (old-states (cons current-state old-states)))
              (dfs2 paths old-states successors goal?))))))


;;; One problem is that dfs and dfs2 only return one solution to the puzzle. Moreover,
;;; if several solutions exist, there is no guarantee that the solution returned
;;; is the best, or shortest one. Here we will extend dfs to return all possible
;;; solutions.

(define (dfs-all paths successors goal? solutions)
;  (display paths) (display " ") (display solutions) (newline)
  (if (null? paths)
      solutions
      (let* ((current-path (car paths))
             (current-state (car current-path)))
        (if (goal? current-state)
            (dfs-all (cdr paths) successors goal? (cons (reverse current-path) solutions))
            (let* ((adjacent-states (successors current-state))
                   (new-states (set-difference adjacent-states current-path))
                   (new-branches (map (lambda (x) (cons x current-path)) new-states))
                   (new-paths (append new-branches (cdr paths))))
              (dfs-all new-paths successors goal? solutions))))))

;;; Lets adapt the above to peg solitaire, with 4 red pegs and 4 blue pegs on
;;; a board with 9 holes.


;; (repeat s n) returns a list that contains n copies of expression s.
;; Thus (repeat 'apples 3) -> (apples apples apples).
(define (repeat s n)
  (if (zero? n)
      '()
      (cons s (repeat s (sub1 n)))))

;;; The initial state of the one-dimensional peg solitaire puzzle of size n is
;;; represented by a single list, containing n Rs followed, by 1 E, followed by n Bs.
;;; Thus (make-initial-ps-state 3) -> (R R R E B B B).
(define (make-initial-ps-state n)
  (append (repeat 'R n) (list 'E) (repeat 'B n)))

;;; One operation for each kind of legal move:

;;; (red-step s) accepts a list that represents a legal state of one-dimensional peg solitaire
;;; puzzle, and attempts to apply a red-step operation to that state. If a red-step is allowed,
;;; then the new state is returned. Otherwise, #f is returned. Thus,
;;; (red-step '(R R E B B)) -> (R E R B B), and
;;; (red-step '(B B E R R)) -> #f.

(define (red-step-aux in out)
  (cond ((or (null? in) (null? (cdr in))) #f)                ; s must contain at least two symbols to procede.
        ((and (eq? (car in) 'R) (eq? (cadr in) 'E))          ; s matches (R E *)
         (append (reverse out) '(E R) (cddr in)))
        (else (red-step-aux (cdr in) (cons (car in) out))))) ; s does not match (R E *)

(define (red-step s)
  (red-step-aux s '()))

(define (red-jump-aux in out)
  (cond ((or (null? in) (null? (cdr in)) (null? (cddr in))) #f)        ; s must contain at least three symbols to procede.
        ((and (eq? (car in) 'R) (eq? (caddr in) 'E))                ; s matches (R ? E *)
         (append (reverse out) (cons 'E (cons (cadr in) (cons 'R (cdddr in))))))
        (else (red-jump-aux (cdr in) (cons (car in) out)))))        ; s does not match (R ? E *)

(define (red-jump s)
  (red-jump-aux s '()))

(define (blue-step-aux in out)
  (cond ((or (null? in) (null? (cdr in))) #f)
        ((and (eq? (car in) 'E) (eq? (cadr in) 'B))
         (append (reverse out) '(B E) (cddr in)))
        (else (blue-step-aux (cdr in) (cons (car in) out)))))

(define (blue-step s)
  (blue-step-aux s '()))

(define (blue-jump-aux in out)
  (cond ((or (null? in) (null? (cdr in)) (null? (cddr in))) #f)
        ((and (eq? (car in) 'E) (eq? (caddr in) 'B))
         (append (reverse out) (cons 'B (cons (cadr in) (cons 'E (cdddr in))))))
        (else (blue-jump-aux (cdr in) (cons (car in) out)))))

(define (blue-jump s)
  (blue-jump-aux s '()))


(define (ps-successors s)
  (filter identity
          (map (lambda (f) (f s))
               (list red-jump red-step blue-step blue-jump))))

(define (peg-solitaire n)
  (let* ((initial-state (make-initial-ps-state n))
         (goal-state (reverse initial-state)))
    (dfs2 (list (list initial-state)) '() ps-successors (lambda (x) (equal? x goal-state)))))

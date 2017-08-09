;;; graphsearch.scm
;;; Created for CS 32, October 27, 2004.
;;;
;;; Copyright (C) Robert R. Snapp 2004

;;; A graph consists of a list of lists. Each sublist contains a vertex label, followed by a list
;;; of edges, each edge represtented by the vertex of the destination.

;;; *tiny* is a tiny graph
(define *tiny*
  '((a (b c)) (b (a)) (c (a))))

;;; *maze* is the graph of a maze.
(define *hampton-court*
  '((a (b)) (b (a c d)) (c (b))
    (d (b e f)) (e (d)) (f (d g h))
    (g (f)) (h (f i j)) (i (h j l))
    (j (h i k)) (k (j)) (l (i m n))
    (m (l)) (n (l o p)) (o (n)) (p (n))))

(define *coxeter*
  '((a (b c)) (b (a b b)) (c (a h d)) (d (c e f)) (e (d g p))
    (f (d x r)) (g (e lambda j)) (h (c)) (i (lambda)) (j (g))
    (k (lambda l k k m)) (l (l l k n)) (m (k)) (n (l q n n))
    (o (v)) (p (e)) (q (n nu eta)) (r (f s pi)) (s (r s s))
    (t (u z alpha)) (u (t w v)) (v (u beta o)) (w (u w w))
    (x (f)) (y (z)) (z (y t alpha)) (alpha (t z gamma))
    (beta (gamma rho v)) (gamma (alpha beta mu)) (delta (nu))
    (epsilon (mu)) (eta (q)) (lambda (k g i)) (mu (gamma epsilon nu))
    (nu (delta mu q)) (pi (r)) (rho (beta))))


;; The goal of the maze is vertex 'o. goal? returns #t if and only if the argument is the goal vertex.
(define goal?
  (lambda (vertex)
    (eq? vertex 'o)))

;; search accepts two arguments, history and graph, where history is a list of vertices that link the current-vertex
;; with the start of the maze, and graph is a list representation of a graph that contains only the edges that have
;; not yet been explored. This function searches for the goal node of the graph following an algorithm similar
;; to Tremaux's.algorithm for threading a maze. Instead of marking the entrances and exits with N's and X's, pairs
;; of directed edges are removed from the graph as they are traversed. Edges that should be labeled with an X are
;; added (cons'ed) to the history list, for easy backtracking.
;;
;; Ultimately, search should return a solution path, i.e. a list of vertex symbols, that describes a
;; direct path from the starting vertes to the goal vertex.
;;
;; Syntax: (search '(a) *hampton-court*) -> (a b d f h i l n o)

(define search
  (lambda (history graph)
    (cond ((null? history) (display "Search failed!") '()) ; Failure!!!
         (else (let* ((current-vertex (car history))
                    (new-vertex (pick-new-path current-vertex graph)))
                 (cond ((not new-vertex) (search (cdr history) graph)) ; no open path => backtrack
                      (else (let ((new-history (cons new-vertex history))
                                (new-graph (remove-edge current-vertex new-vertex
                                            (remove-edge new-vertex current-vertex
                                               (mark-visited new-vertex graph)))))
                       (cond ((goal? new-vertex) (display "Eureka!") (reverse new-history)) ; Success!!!
                            ((visited? new-vertex graph) (search history new-graph)) ;found old junction => backtrack
                            (else (search new-history new-graph))))))))))) ; found new junction => forward search

;; remove-edge requires three arguments: the source vertex of a directed edge (src),
;; the destination vertex of that directed edge (dst), and the current state of the
;; graph (graph).
;;
;; It returns the original graph after removing the indicated directed edge.
;; Syntax (remove-edge 'a 'c *tiny*) -> ((a (b)) (b (a)) (c (a)))

(define remove-edge
  (lambda (src dst graph)
    (let ((remove
           (lambda (s)
              (cond ((eq? src (car s))
                     (cons src (cons (remv1 dst (cadr s)) (cddr s))))
                   (else s)))))
      (map remove graph))))

;; pick-new-path returns the label of an open edge (i.e., an unlabeled passage) from the indicated
;; vertex in the indicated graph. If no edge is open then the boolean value #f is returned.
;;
;; Syntax: (pick-new-path 'b *hampton-court*) -> a

(define pick-new-path
  (lambda (vertex graph)
    (let* ((vertex-info (get-vertex-info vertex graph))
          (open-paths (cadr vertex-info)))
      (cond ((null? open-paths) #f) ; No open paths, return #f
           (else (car open-paths))))))

;; get-vertex-info return the sublist contained by the graph that corresponds to the indicated
;; target-vertex.
;; Syntax (get-vertex-info 'b *hampton-court*) -> (b (a c d))

(define get-vertex-info
  (lambda (target-vertex graph)
    (cond ((null? graph) (error "vertex not found"))
          ((eq? target-vertex (caar graph)) (car graph))
          (else (get-vertex-info target-vertex (cdr graph))))))

;; mark-visited returns a modified representation of the graph specified by the second argument
;; such that the vertex indicated by the first argument is labed as visited.

(define mark-visited
  (lambda (vertex graph)
    (let* ((mark
            (lambda (s)
              (cond ((eq? vertex (car s)) (list (car s) (cadr s) 'visited)) ;; match!
                   (else s)))))
      (map mark graph))))

;; visted? is a predicate function that returns #t if and only if the vertex indicated by the
;; first argument has been visited in the graph indicated by the second argument.
;;
;; Example (visited? 'b (mark-visited 'b *hampton-court*)) -> #t

(define visited?
  (lambda (vertex graph)
    (let ((vertex-info (get-vertex-info vertex graph)))
      (not (null? (cddr vertex-info)))))) ;; Assume visited nodes have sublists of length 3 (or more).

;; remv1 removes the first occurence of the first-argument from the list indicated by the
;; second argument.
;;
;; Syntax (remv1 'ant '(bee ant fly ant gnat)) -> (bee fly ant gnat).

(define (remv1 a l)
  (cond ((null? l) '())
       ((eq? a (car l)) (cdr l)) ; match! so drop the car.
       (else (cons (car l) (remv1 a (cdr l))))))
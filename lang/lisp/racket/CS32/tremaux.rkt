;;; tremaux.rkt
;;;
;;; Robert R. Snapp (C) 2006, 2008, 2010.
;;; snapp@cs.uvm.edu
;;;
;;; Functions in this file apply Tre'maux's algorithm to a finite maze with
;;; unique start and goal nodes. The main function is called tremaux, accepts
;;; three arguments:
;;;
;;;    (tremaux start goal graph)
;;;
;;; The first argument start indicates the vertex label of the start node.
;;; The second argument goal indicates the vertex label of the goal node.
;;; The third argument graph should evaluate to a nested, list representation
;;; of the graph of the maze, in which each sublist begins with a vertex label,
;;; and the second sublist item is a list of that vertex's nearest neighbors
;;; in the graph. For the example defined below, one might invoke
;;;
;;;    (tremaux 'a 'o *hampton-court*)
;;;
;;; This program has a test suite, defined at the end, which will be evaluated
;;; if the following variable is set to #t
(define *execute-test-suit* #t)

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;;; A graph consists of a list of lists. Each sublist contains a vertex label,
;;; followed by a list of edges, each edge represtented by the vertex of the
;;; destination. A weakness of this representation is that it does not clearly
;;; differentiate between duplicate paths that connect the same the same pair
;;; of vertices. In this case, auxillary marks can be inserted.

;;; The test suite requires that *maze* define a valid graph, that contains
;;; vertices 'a (start) and 'o (goal). Here we assume the Hampton Court maze.
(define *hampton-court*
  '((a (b)) (b (a c d)) (c (b))
            (d (b e f)) (e (d)) (f (d g h))
            (g (f)) (h (f i j)) (i (h j l))
            (j (h i k)) (k (j)) (l (i m n))
            (m (l)) (n (l o p)) (o (n)) (p (n))))

(define *shallow*
  '((a (b c d e f)) (b (a)) (c (a)) (d (a)) (e (a)) (f (a))))

(define *loopy*
  '((a (b c d e)) (b (a g)) (c (a)) (d (a g)) (e (a)) (g (b d))))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; Get neighbors returns the list of vertices that are adjacent to the indicated
;; vertex. Thus, (get-neighbors 'i *hampton-court*) => (h j l).
(define (get-neighbors node graph)
  (cond ((null? graph) '()) ; did not find node in the graph.
        ((eq? node (caar graph)) (cadar graph))
        (else (get-neighbors node (cdr graph)))))

;; When applying Tre'maux's algorithm, marks like 'X and 'N are written at the
;; path entrances and exits. A record of all such marks will be maintained
;; in a list called marks. Each element of that list is a nested pair of the
;; form '((b . a) . X)). The first pair indicates the location of the mark,
;; the second pair indicates the label. Here the location indicates that path
;; at vertex b, that leads to vertex a. This in particular would be the first
;; label drawn for the Hampton Court maze.

;; new-node? returns #t if the indicated node has not yet been visited, i.e.,
;; there is no reference to it in marks.
(define (new-node? node marks)
  (cond ((null? marks) #t)            ; no marks exist at node.
        ((eq? node (caaar marks)) #f) ; is the next mark at node?
        (else (new-node? node (cdr marks)))))

;; get-entry returns a dotted pair in the form (a . b) that represents the
;; path from a, the indicated node, to node b, along a path that has not yet
;; been used in the search. If no such path exists, then get-entry returns #f.
(define (get-entry node neighbors marks)
  (if (null? neighbors)
      #f ; failure. no path adjacent to node is unmarked.
      (let ((candidate-entry (cons node (car neighbors))))
        (if (not (assoc candidate-entry marks))
            candidate-entry ; success!
            (get-entry node (cdr neighbors) marks)))))

;; get-exit returns the path at the indicated old node that is labeled
;; with an 'X. A dotted pair of the form (a . b) is returned, where a is the
;; indicated node, and b indicates the destination of the path. If no exit
;; path exists then #f is returned.
(define (get-exit node neighbors marks)
  (if (null? neighbors)
      #f ; failure: no marked exit is adjacent to node.
      (let* ((exit (cons node (car neighbors)))
             (mark (assoc exit marks)))  ; mark can be #f or ((a . b) . X)
        (if (and mark (eq? (cdr mark) 'X))
            exit
            (get-exit node (cdr neighbors) marks)))))

;; add-X inserts a new X-label for the indicated path-end into the front
;; of the indicated a-list marks.
(define (add-X exit marks) ; exit should be of the form (p . q)
  (cons (cons exit 'X) marks))

;; add-N inserts a new N-label for the indicated path-end into the front
;; of the indicated a-list marks.
(define (add-N entry marks) ; entry should be of the form (p . q)
  (cons (cons entry 'N) marks))

;; tremaux implements Tre'maux's algorithm for the indicated graph. The first
;; argument, start, indicates the label of the starting node; the second, goal,
;; indicates that of the goal node. The third argument, graph, indicates the
;; graph of the maze that should contain start and goal as nodes. (Note that
;; the algorithm assumes that no more than one passage exists between any pair
;; of nodes, and that no passage loops back to its original junction. If a graph
;; has more than one edge between a pair of vertices, then new vertices should
;; be inserted within the paths, as needed, to obtain uniquely labeled paths.)
(define (tremaux start goal graph)
  (letrec ([start? (lambda (v) (eq? v start))] ; #t if v is start node
           [goal? (lambda (v) (eq? v goal))]   ; #t if v is goal node
           [dead-end? (lambda (v neighbors)    ; #t if v is dead end
                        (and (not (start? v))
                             (not (goal? v))
                             (= (length neighbors) 1)))]
           [forward (lambda (path marks)
                      (let* ([node (car path)]
                             [neighbors (get-neighbors node graph)]
                             [last (if (null? (cdr path)) start (cadr path))]
                             [exit (cons node last)])
                        (cond ((goal? node)                      ; Eureka!
                               (tremaux-output start goal graph path
                                               (add-X exit marks)))
                              ((dead-end? node neighbors)         ; turn around &
                               (backward (cons last path) marks)) ; march backward
                              ((new-node? node marks)
                               (let* ([marks (if (start? node) ; add X @ exit
                                                 marks
                                                 (add-X exit marks))]
                                      [entry (get-entry node neighbors marks)])
                                 (forward (cons (cdr entry) path) ; add N @ entry
                                          (add-N entry marks))))  ; march forward.
                              (else (backward (cons (cadr path) path) ; add N @ exit
                                              (add-N exit marks))))))]; march backward
           [backward (lambda (path marks)
                       (let* ([node (car path)]
                              [neighbors (get-neighbors node graph)]
                              [entry (get-entry node neighbors marks)])
                         (cond (entry (forward (cons (cdr entry) path); if new path,
                                               (add-N entry marks)))  ; add N @ entry,
                                                                      ; march forward
                               ((start? node)                         ; Abort search
                                (display "Search Failed") (newline) #f)
                               (else (let ([exit (get-exit node neighbors marks)])
                                       (backward (cons (cdr exit) path) ; follow X mark
                                                 marks))))))])          ; march backward
    (forward (list start) '())))

;; tremaux-output is invoked after the goal is reached. The value it returns is ultimately
;; returned by tremaux, but it also displays the list of marks drawn, and can be modified
;; as desired.
(define (tremaux-output start goal graph path marks)
  (letrec ([start? (lambda (node) (eq? node start))]
           [escape (lambda (exit-path marks)
                     (let* ((node (car exit-path))
                            (neighbors (get-neighbors node graph))
                            (exit (get-exit node neighbors marks)))
                       (cond ((start? node) exit-path)
                             (else (escape (cons (cdr exit) exit-path) marks)))))])
    (display (reverse marks))
    (newline)
    (list (escape (list goal) marks) (reverse path))))

;;;;;; Debugging tools for checking the consistency of a graph
;;;;;; Contributed by Paul Haake:
;;
;; Syntax: (check-consistency *maze*)  -> nil, but prints any label inconsistencies.
;;
;; The function prints two kinds of inconsistencies in a maze:
;;   1. If a node v is a neighbor of w but w is not a neighbor of v.
;;   2. If a node v is a neighbor of itself.
;; v is a node, es is its neighbors, and l is the maze.
(define (members-maze v es l)
  (if (not (null? es))
      (begin
        (if (not (memq v (get-neighbors (car es) l)))
            (printf "Node ~s is a neighbor of ~s, but not vice versa.\n" (car es) v))
        (if (eq? v (car es))
            (printf "Node ~s should not be a neighbor of itself.\n" v))
        (members-maze v (cdr es) l))))

;; Prints any inconsistencies in the maze.  If the maze is OK, it prints nothing.
(define (check-consistency graph)
  (letrec ([check-cons-recurse
            (lambda (n graph)
              (if (not (= n (length graph)))
                  (let* ((e (list-ref graph n))
                         (v (car e))
                         (es (cadr e)))
                    (and (members-maze v es graph) (check-cons-recurse (+ n 1) graph)))))])
    (check-cons-recurse 0 graph)))

;;; Optional test suite
(if *execute-test-suit*
    (begin
      (display "Executing optional test suite:\n")
      (display "(tremaux 'a 'o *hampton-court*) => ")
      (newline)
      (display (tremaux 'a 'o *hampton-court*))
      (newline) (newline)
      (display "(tremaux 'a 'e *shallow*) => ")
      (newline)
      (display (tremaux 'a 'e *shallow*))
      (newline) (newline)
      (display "(tremaux 'a 'e *loopy*) => ")
      (newline)
      (display (tremaux 'a 'e *loopy*))
      ))




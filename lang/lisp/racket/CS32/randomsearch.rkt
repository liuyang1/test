;; randomsearch.scm
;;
;; Copyright (C) Robert R. Snapp 2005
;;

;; *maze* represents the graph of the Hampton Court hedge maze as a
;; nested list

(define *maze* '((a (b)) (b (a c d)) (c (b)) (d (b e f))
                 (e (d)) (f (d g h)) (g (f)) (h (f i j))
                 (i (h j l)) (j (h i k)) (k (j)) (l (i m n))
                 (m (l)) (n (l o p)) (o (n)) (p (n))))

;; The function neighbors returns a list of the vertices that are next to a given vertex.
;; Thus (neighbors 'j *maze*) -> (h i k).  Try it out.
(define neighbors
   (lambda (vertex graph)
      (cond ((null? graph) '())
            ((eq? vertex (caar graph)) (cadar graph))
            (else (neighbors vertex (cdr graph))))))

;; The function count-neighbors returns the number of vertices that are next to a given
;; vertex. Thus, (count-neighbors 'j *maze*) -> 3
(define count-neighbors
   (lambda (vertex maze)
      (length (neighbors vertex maze))))

;; get-random-neighbor returns one of the neigbors of the given vertex.
;; Thus (get-random-neighbor 'j *maze*) returns one of h, i, or k.

;; get-random-neighbor has be rewritten so that the graph is only searched once.
;; Note that count-neigbors is no longer used.
(define get-random-neighbor
  (lambda (v graph)
    (let ((neighborhood (neighbors v graph)))
      (list-ref neighborhood (random (length neighborhood))))))

;; randomsearch implements an agent that threads a maze, starting at the vertex
;; start, and ending at the vertex goal. Thus, (randomsearch 'a 'o *maze*)
;; might display the vertex labels "abdfgfhjhfgfhilnpnlmlnpno". randomsearch
;; though, returns the empty list ().
(define randomsearch
  (lambda (start goal graph)
    (display start)
    (cond ((eq? start goal) '())
          (else (randomsearch
                 (get-random-neighbor start graph) goal graph)))))

;; Test
(display "Test: evaluating (randomsearch 'a 'o *maze*)")
(newline)
(randomsearch 'a 'o *maze*)
(newline)

;; randomsearch2 also implements a random search, but maintains a list of the
;; vertices visited in the list history. Because (car history) contains the
;; current (or starting) vertex, it should be invoked with the syntax:
;;
;;   (randomsearch2 '(a) 'o *maze*)
;;
(define randomsearch2
  (lambda (history goal graph)
    (cond ((eq? (car history) goal) (reverse history))
          (else (randomsearch2
                 (cons (get-random-neighbor (car history) graph) history)
                 goal
                 graph)))))

;; Test
(display "Test: evaluating (length (randomsearch2 '(a) 'o *maze*)):")
(newline)
(length (randomsearch2 '(a) 'o *maze*))
(newline)
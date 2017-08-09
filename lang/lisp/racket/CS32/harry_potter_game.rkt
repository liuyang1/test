#lang racket

;;; The Harry Potter Game
;;; Created by Jeremy Gould, University of Vermont, for CS 32,
;;; with assistance from Robert Snapp, and incorporating old
;;; code contributed by Paul Haake.

;;; This file combines a graph-analysis toolkit for finding directed
;;; paths in connected graphs, with a graph-based adventure game
;;; designed with a Harry Potter theme.

(define *test-mode* #t)

;***************************************************************************************************

;  List Representations of Graphs

;***************************************************************************************************

;;; Here we define the graph of the adventure game. Each labeled vertex corresponds to a
;;; store in Diagon Alley, described further down.

(define *Diagon-Alley*
  '((a (b d)) (b (a e c)) (c (b k))
            (d (a f h)) (e (b g)) (f (d i))
            (g (e j)) (h (d i)) (i (h k l))
            (j (g k)) (k (j c i)) (l (i m))
            (m (l))))

;***************************************************************************************************

;  Graph Utility Functions

;***************************************************************************************************
;;; Our neighbors function returns the set of nodes adjacent to any given
;;; node. Thus (neighbors 'i *Diagon-Alley*) returns (h j l).

(define (neighbors node graph)
  (cond ((null? graph) '())
        ((eq? (caar graph) node) (cadar graph))
        (else (neighbors node (cdr graph)))))

;;; A goal predicate accepts a single argument that represents the state
;;; of the puzzle, and returns #t if and only if the argument describes
;;; a state that matches the goal. Here, da-goal? returns #t only when
;;; the value of the argument matches the goal node in the maze, 'm.

(define (da-goal? node)
  (eq? node 'm))

;;; A successors function accepts a single argument that represents the
;;; current state, and returns a list of all states that can be reached
;;; in a single move. Here da-successors returns the list of neighbors
;;; of the given node.
(define (da-successors node)
  (neighbors node *Diagon-Alley*))


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
;;; represents the intial state, e.g., '((a)) for Diagon Alley. Then as the
;;; function recurs, the paths are extended. Some paths may fork into multiple
;;; paths at junctions, and some will eventually vanish at dead ends.
;;;
;;; The second argument, old-states, is list that represents the set of states that
;;; have been visited. In the context of the maze, this would be the set of nodes
;;; that have been visited during the course of the search. The initial value should
;;; always be an empty list.
;;;
;;; successors represents a successors function as described above, e.g. da-successors.
;;;
;;; goal? represents a goal predicate as described above, e.g., da-goal?

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
;;;;;

(cond (*test-mode* (dfs '((a)) '() da-successors da-goal?)))


;;; The above should return (a b e g j k i l m), a direct path that connects the
;;; entrance with the goal.

;;; The inner workings of the dfs function might be clearer if we employ the let
;;; function, which enables us to bind temporarily values to symbols with informative
;;; names

(define (dfs2 paths old-states successors goal?)
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

;;; Let's test it again.

(cond (*test-mode* (dfs2 '((a)) '() da-successors da-goal?)))

;;; Likewise, the above should return (a b e g j k i l m), a direct path that
;;; connects the entrance with the goal.

;;; We can now define a more elaborate function called theseus that threads a general
;;; maze using dfs

(define (theseus start goal graph)
  (define (goal? v)
    (eq? v goal))
  (define (successors v)
    (neighbors v graph))
  (dfs2 (list (list start)) '() successors goal?))

(cond (*test-mode*
    (begin
      (newline) (display "Testing (theseus 'a 'm *Diagon-Alley*)...") (newline)
      (theseus 'a 'm *Diagon-Alley*))))


;;; Alternatively, the internally defined functions goal? and successors
;;; can be specified by using let

(define (theseus2 start goal graph)
  (let ((goal? (lambda (v) (eq? v goal)))
        (successors (lambda (v) (neighbors v graph))))
    (dfs2 (list (list start)) '() successors goal?)))

;;; One more test.
(cond (*test-mode*
    (begin
       (newline)
       (display "Testing (theseus2 'a 'm *Diagon-Alley*), one more time...")
       (newline)
       (theseus2 'a 'm *Diagon-Alley*))))

;;; One problem is that dfs only returns one solution to the puzzle. Moreover,
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



;(dfs-all '((a)) da-successors da-goal? '())


;;;;;; Debugging tool for checking the consistency of a graph
;;;;;; Contributed by Paul Haake:
;;
;; Syntax: (check-consistency *maze*)  -> nil, but prints any label inconsistencies.
;;
;; The function prints two kinds of inconsistencies in a maze:
;;   1. If a node v is a neighbor of w but w is not a neighbor of v.
;;   2. If a node v is a neighbor of itself.
;; v is a node, es is its neighbors, and l is the maze.
;; Prints any inconsistencies in the maze.  If the maze is OK, it prints nothing.
(define (check-consistency graph)
 (letrec ((check-cons-recurse
           (lambda (n graph)
             (and (not (= n (length graph)))
                 (let* ((e (list-ref graph n))
                        (v (car e))
                        (es (cadr e)))
                   (and (members-maze v es graph) (check-cons-recurse (+ n 1) graph)))))))
   (check-cons-recurse 0 graph)))

(define (members-maze v es l)
 (and (not (null? es))
     (begin
       (and (not(memq v (neighbors (car es) l)))
           v)
       (and (eq? v (car es))
           v)
       (members-maze v (cdr es) l))))
;**********************************************************************************************************

;    Functions for manupulating associative lists
;    These are used by the adventure GUI

;**********************************************************************************************************

;; node->description

;  in:    s - an associative list of nodes with their string descriptions
;         n - a node to search the list for

;  out:   String from s = (..... (n. String) ... ) if n exists in s
;         otherwise the string equivelent of the node n itself
(define (node->description n s)
  (let ((title (assoc n s)))
    (if title
        (cdr title)
        (symbol->string n))))

;; description->node

;  in:    s - an associative list of nodes with their string descriptions
;         n - a description to search the list for

;  out:   Symbol x from s = (..... (x. n) ... ) if String n exists in s
;         otherwise the symbol equivelent of the string n itself
(define (description->node n s)
  (if (null? s)
      (string->symbol n)
      (if (eq? (cdar s) n)
          (caar s)
          (description->node n (cdr s)))))


; node-list->desc-list
; in:    s = a list of nodes
;        titles = an associative list of nodes with their string descriptions

; out:   a list of the string descriptions of the nodes
;        (if a node is not in titles its title is the string representation of its symbol)
(define (node-list->desc-list s titles)
  (if (null? s)
      '()
      (cons (node->description (car s) titles) (node-list->desc-list (cdr s) titles))))

; symbol-list->string-list
; in:    a list of symbols
; out:   a list of the string representation of the symbols
(define (symbol-list->string-list s)
  (if (null? s)
      '()
      (cons (symbol->string (car s)) (symbol-list->string-list (cdr s)))))


;***************************************************************************************************

;  Associative Lists
;  These lists permit us to map data (in this case natural english descriptions) to nodes

;***************************************************************************************************

(define *Diagon-Alley-shops*
  '((a . "Gringotts Wizarding Bank")
    (b . "Madam Malkin's Robes for all Ocassaions")
    (c . "Ollivander's")
    (d . "Weasley's Wizard Wheezes")
    (e . "Quality Quidditch Supplies")
    (f . "Magical Menagerie")
    (g . "Florean Fortescue's Ice Cream Parlour")
    (h . "Eyelops Owl Emporium")
    (i . "Flourish and Blotts")
    (j . "Leaky Cauldron")
    (k . "Potage's Cauldron Shop")
    (l . "Knockturn Alley")
    (m . "Borgin & Burkes")))


(define *shop-descriptions*
  '((a .
"There is no safer place to keep your money!")
    (b .
"Come and get your new school robes.")
    (c .
"Making Wands since before you were born.")
    (d .
"The best Jokes in the World! Would you like a toy wand?")
    (e .
"Just check out our window display. People are drooling at our merchandise.")
    (f .
"What kind of magic pet do you want?.")
    (g .
"Magic Ice Cream anyone?")
    (h .
"Cover your head. It's dirty in here.")
    (i .
"Anyone would want to read these books.")
    (j .
"Dark and dusty, but still super famous. Come and have a drink!")
    (k .
"Copper, Brass, Pewter, Silver")
    (l .
"DON'T COME HERE!")
    (m .
"You're just asking for trouble by coming in here! Go away you freak.")))


;**********************************************************************************************************
;
;    Pure GUI stuff (Not very interesting or useful.  Intended to be used, not analyzed.)
;                But feel free to take a look! :P
;
;**********************************************************************************************************
(require framework/test) ; Just your friendly neighborhood GUI library
(require racket/draw)
(require racket/gui)

(define monster-font
  (make-object font%
                14
          'swiss))

;; adventure_window
;
;   in:    graph - a graph in our standard nested list format = ( (node (neighbor1, neighbor2, ....)) ....)
;          node - the currently "occupied" node
;          titles - an associative list of the titles of each node
;          descriptions - an associative list of the descriptions of each node
;          x - window x axis pos
;          y - window y axis pos
;          width - window width
;          height - window height

;   out:   A GUI interface which diplays the current node and permits the user to select one of that node's
;          neighbors in a radio button menu.
;          Also provides a button which when clicked closes this instance of adventure and creates a new one
;          "occupying" the selected node. (new window same size, position same as old one)

(define (adventure_window graph node titles descriptions x y width height)
; top level frame
(define topframe (new frame% [label "Graph Traversal - Adventure Mode"]
                ;[width width]
            ;[height height]
            [x x]
            [y y]
            ;[style style]
            ;[enabled enabled]
            ;[border border]
            ;[spacing spacing]
            [alignment '(center center)]
            [width width]
            [height height]
            ;[stretchable-width #t]
            ;[stretchable-height #t]
                ))

; Top level pane to contain control and display elements
(define toppane (new vertical-pane%
            [parent topframe]
            [stretchable-width #t]
            [stretchable-height #t]
                [alignment '(center center)]))

; Display the node title and description
(define midtitle (new text-field%
                      [label (string-append "Your current location:    " (node->description node titles))]
                      [parent toppane]
                      [init-value (node->description node descriptions)]
                      [vert-margin 2]
                      [horiz-margin 2]
                      [stretchable-width #f]
                      [stretchable-height #t]
                      [style '(multiple vertical-label)]
                      [font monster-font]
                      [enabled #f]

                      ))
;(send midtitle set-value (node->description node descriptions))


; Display the node's neighbors in a radio box
(define radiochoice (new radio-box%
            [label "From here you can travel to:"]
                ;[choices (symbol-list->string-list (neighbors node graph) ) ]
            [choices (node-list->desc-list (neighbors node graph) titles) ]
            [parent toppane]
                [style '(vertical vertical-label)]
                [stretchable-width #f]
                [stretchable-height #f]
                [font monster-font]))

; Finally, display a button used to go to selected neighbor
(define gobutton (new button% [parent toppane]
              [label "Go!"]
             ; Callback procedure for a button click:
             [callback (lambda (button event)
                         (adventure_window
                                    graph
                                    (description->node (send radiochoice get-item-label (send radiochoice get-selection)) titles)
                                    titles
                                    descriptions
                                    (send topframe get-x)
                                    (send topframe get-y)
                                    (send topframe get-width)
                                    (send topframe get-height)
                                    )
                         (test:close-top-level-window topframe)
                         )]))


; Show the frame by calling its show method
(send topframe show #t)

)

; A conenient handle to call adventure window with some default window dimensions
(define (adventure graph node titles descriptions)
  (adventure_window graph node titles descriptions 0 0 800 600))


(define (play)
  (adventure *Diagon-Alley* 'a *Diagon-Alley-shops* *shop-descriptions*))


;Delete me!

;(adventure *hampton-court* 'a '() '()) ;Stub!
;(adventure *hampton-court* 'a *hampton-titles* *hampton-descriptions*) ;Stub!
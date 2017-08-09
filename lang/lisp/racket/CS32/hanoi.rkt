#lang racket

;;; hanoi.rkt
;;;
;;; The function hanoi simulates solutions to Lucas's Tower of Hanoi puzzle.
;;;
;;; Written for CS 32, Fall 2010
;;; Copyright 2003, 2004, 2005, 2010 Robert R. Snapp
;;;
;;; Evaluate this file in DrRacket with language PrettyBig, which defines the
;;; functions error and printf.

;; Concise representation: The state of the Tower of Hanoi puzzle is
;; represented by a single list of integers.  The first integer represents
;; the peg (1, 2,  or 3) on which the smallest disk rests, the second
;; integer, the peg on which the second smallest disk rests, and so, on.
;;
;; Thus, the initial state is (1 1 ... 1), and the goal state is (3 3 ... 3).
;;
(define (make-list n value)
    (if (zero? n)
        '()
        (cons value (make-list (- n 1) value))))

;; remove-all : symbol list -> list
;; removes every occurence of symbol s from the list lst.
;; (This function is like anteater.)
;;
;; Examples: (remove-all 'a '(b a c a b)) -> (b c b), and
;; (remove* 'z '(b a c a b)) -> (b a c a b).
;;
;; Actually this function is not used below as racket has a built-in function
;; called remove that removes the first occurence of the symbol form a list.
;; that is (remove 'a '(b c a d a)) => (b c d a). remove is all we need below.
(define (remove-all s lst)
    (cond ((null? lst) '()); an empty list
          ((eq? s (car lst)) (remove-all s (cdr lst)))
          (else (cons (car lst) (remove-all s (cdr lst))))))

;; Creates an instance of the tower of Hanoi puzzle with n disks on one
;; tower, and no disks on the remaining two towers.
(define (initial-state n)
    (make-list n 1))

;; Returns true if the smallest disk on peg src-peg can be moved to
;; peg dst-peg, i.e., if the first occurance of the value (second m)
;; appears after the first occurance of the value (first m) in the state
;; representation.
(define (move-check? src-peg dst-peg state)
    (cond ((null? state) #f) ; There is no disk on peg (first m)!
          ((eq? (car state) src-peg) #t)
          ((eq? (car state) dst-peg) #f)
          (else (move-check? src-peg dst-peg (cdr state)))))

;; Attempt to move the disk that is at the top of peg src-peg into the
;; top position of peg dst-peg. If the move is legal, then the new state
;; will is returned. Otherwise, the original state is returned.
(define (move-disk src-peg dst-peg state)
    (cond ((null? state) '())
          ((eq? (car state) dst-peg) state) ; Move is illegal!
          ((eq? (car state) src-peg) (cons dst-peg (cdr state))) ; Move is legal.
          (else (cons (car state) (move-disk src-peg dst-peg (cdr state))))))


;; (define (error tag str)
;;  (display tag)
;;  (display str))

;; Applies a sequence of moves to the indicated state.
(define (sequential-move move-list state)
    (if (null? move-list) ; Is the move-list empty?
        state
        (let* ((next-move (car move-list))
               (src-peg (car next-move))
               (dst-peg (cadr next-move))
               (next-state (move-disk src-peg dst-peg state)))
          (if (move-check? src-peg dst-peg state)
              (begin
                (printf "Moving smallest disk from peg ~s onto peg ~s in state ~s yields state ~s.\n"
                        src-peg dst-peg state next-state)
                (sequential-move (cdr move-list) next-state))
              (error 'sequential-move "Illegal move.")
              ))))

;; agent: integer integer n -> list
;; agent generates a list of legal moves in the form:
;;
;; ((src-peg1 dst-peg1) ... (src-pegn dst-pegn))
;;
;; that moves a stack of k disks from the home-peg to the target-peg.
;;
;; Example:
;;  (agent 1 3 3)->((1 3) (1 2) (3 2) (1 3) (2 1) (2 3) (1 3))

(define (agent home-peg target-peg n)
  (let ((spare-peg (car (remove-all home-peg (remove-all target-peg '(1 2 3))))))
    (if (<= n 0)
        '() ; an empty list
        (append (agent home-peg spare-peg (- n 1))
                (list (list home-peg target-peg))
                (agent spare-peg target-peg (- n 1))))))


;; hanoi: integer -> list
;; solves and simulates the shortest solution to the tower of hanoi puzzle with n disks.
;; Since the function sequential-move is invoked, each intermediate state is displayed.
;; The final state, a list of n threes, is returned.
;;
;; Example (hanoi 3), displays
#|
Initial state: (1 1 1).
Moving smallest disk from peg 1 onto peg 3 in state (1 1 1) yields state (3 1 1).
Moving smallest disk from peg 1 onto peg 2 in state (3 1 1) yields state (3 2 1).
Moving smallest disk from peg 3 onto peg 2 in state (3 2 1) yields state (2 2 1).
Moving smallest disk from peg 1 onto peg 3 in state (2 2 1) yields state (2 2 3).
Moving smallest disk from peg 2 onto peg 1 in state (2 2 3) yields state (1 2 3).
Moving smallest disk from peg 2 onto peg 3 in state (1 2 3) yields state (1 3 3).
Moving smallest disk from peg 1 onto peg 3 in state (1 3 3) yields state (3 3 3).
|#

(define (hanoi n)
  (let ((state (initial-state n)))
    (printf "Initial state: ~s.\n" state)
    (sequential-move (agent 1 3 n) state)))


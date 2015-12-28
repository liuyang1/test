;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-beginner-reader.ss" "lang")((modname less-than-five-starter) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor repeating-decimal #f #t none #f ())))

;; less-than-five-starter.rkt


;PROBLEM:
;
;DESIGN function that consumes a string and determines whether its length is
;less than 5.  Follow the HtDF recipe and leave behind commented out versions 
;of the stub and template.


;; String -> Boolean
;; produce true if length of s is less than 5
(check-expect (less-than-5? "") true)
(check-expect (less-than-5? "four") true)
(check-expect (less-than-5? "12345") false)
(check-expect (less-than-5? "seventy") false)

;(define (less-than-5? s) ;stub
;  true)

;(define (less-than-5? s) ;template
;  (... s))

(define (less-than-5? s)
  (< (string-length s) 5))
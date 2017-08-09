#lang racket

;;; doomsday applies John Conway's Doomsday Algorithm to an arbitrary date
;;; in the Gregorian Calenday.

;;; usage: (doomsday month day year), where month, day and year are integers
;;; example (doomdsay 1 1 2014) returns Wednesday, the day of the week of January 1, 2014.

;;; int-to-day is a simple utility that associates an integer with a day of the week,
;;; starting with 0 for Sunday. It enables us to perform date arithmetic.

(define (int-to-day n)
  (case (modulo n 7)
    ((0) 'Sunday)
    ((1) 'Monday)
    ((2) 'Tuesday)
    ((3) 'Wednesday)
    ((4) 'Thursday)
    ((5) 'Friday)
    ((6) 'Saturday)))

;;; dd-of-year returns the doomsday for the indicated year in the Gregorian calendar.
(define (dd-of-year year)
  (let* ((extra-centuries (floor (/ (modulo year 400) 100)))
         (years-in-century (modulo year 100))
         (twelve-year-steps (floor (/ years-in-century 12)))
         (extra-years (modulo years-in-century 12))
         (extra-leap-years (floor (/ extra-years 4))))
    (+ 2                        ; Tuesday
       (* -2 extra-centuries)   ; Fall back two days for every century beyond the last 400
       twelve-year-steps        ; Advance one day for each group of twelve years since last century.
       extra-years              ; Advance one day for each year since the last 12 year block.
       extra-leap-years)))      ; Advance one extra day for each leap year since the last 12 year block.


;;; leap-year? returns #t if the year is a leap year. Common years result in #f.
(define (leap-year? year)
  (cond ((zero? (modulo year 400)) #t)
        ((zero? (modulo year 100)) #f)
        ((zero? (modulo year 4)) #t)
        (else #f)))

;;; dd-of-mdy implements the doomsday algorithm, but returns an integer code for each day
;;; of the week in accordance with the int-to-day function defined above.
(define (dd-of-mdy month date year)
  (let ((dd (dd-of-year year))
        (leap (if (leap-year? year) 1 0)))
    (case month
      ((4 6 8 10 12) (+ dd (- date month))) ; rule of the even indexed months after Feb.
      ((5 7) (+ dd (- date month 4)))       ; rule for May and July; subtracting 4 to get doomsday.
      ((9 11) (+ dd (- date month 3)))      ; rule for Sept and November: subtracting 3 is like adding 4.
      ((3) (+ dd date))                     ; rule for March
      ((2) (+ dd (- date 28 leap)))         ; rule for February
      ((1) (+ dd (- date 31 leap))))))      ; rule for January

;;; Here is the final function.
(define (doomsday month day year)
  (int-to-day (dd-of-mdy month day year)))


(define (parse-date md-list date year)
  (if (<= date (cadar md-list))
      (list (caar md-list) date year)
      (parse-date (cdr md-list) (- date (cadar md-list)) year)))


(define (random-date)
   (let* ((year (+ (random 1000) 1582))
         (days-in-year (+ 365 (if (leap-year? year) 1 0)))
         (days-in-feb (+ 28 (if (leap-year? year) 1 0)))
         (day (+ 1 (random days-in-year)))
         (md-list (cons '(January 31)
                       (cons (list 'February days-in-feb)
                             '((March 31) (April 30) (May 31) (June 30)
                                          (July 31) (August 31) (September 30)
                                          (October 31) (November 30) (December 31))))))
     (parse-date md-list day year)))

;;; Now we can also generate a random date
(define (random-date-old)
  (let* ((year (+ (random 1000) 1582))
         (days-in-year (+ 365 (if (leap-year? year) 1 0)))
         (days-in-feb (+ 28 (if (leap-year? year) 1 0)))
         (day (+ 1 (random days-in-year))))
    (if (<= day 31)
        (list 'January day year)
        (let ((day (- day 31)))
          (if (<= day days-in-feb)
              (list 'February day year)
              (let ((day (- day days-in-feb)))
                (if (<= day 31)
                    (list 'March day year)
                    (let ((day (- day 31)))
                      (if (<= day 30)
                          (list 'April day year)
                          (let ((day (- day 30)))
                            (if (<= day 31)
                                (list 'May day year)
                                (let ((day (- day 31)))
                                  (if (<= day 30)
                                      (list 'June day year)
                                      (let ((day (- day 30)))
                                        (if (<= day 31)
                                            (list 'July day year)
                                            (let ((day (- day 31)))
                                              (if (<= day 31)
                                                  (list 'August day year)
                                                  (let ((day (- day 31)))
                                                    (if (<= day 30)
                                                        (list 'September day year)
                                                        (let ((day (- day 30)))
                                                          (if (<= day 31)
                                                              (list 'October day year)
                                                              (let ((day (- day 31)))
                                                                (if (<= day 30)
                                                                    (list 'November day year)
                                                                    (list 'December (- day 30) year))))))))))))))))))))))))


(define (ndates n)
  (if (zero? n)
      '()
      (cons (random-date) (ndates (- n 1)))))
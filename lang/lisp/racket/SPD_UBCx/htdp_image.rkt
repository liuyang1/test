;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-beginner-reader.ss" "lang")((modname htdp_image) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor repeating-decimal #f #t none #f ())))
(require 2htdp/image)

(circle 10 "solid" "red")
(rectangle 30 60 "outline" "blue")
(text "hello" 24 "orange")
(above (circle 10 "solid" "red")
       (circle 20 "solid" "yellow")
       (circle 30 "solid" "red"))
(overlay (circle 10 "solid" "red")
         (circle 20 "solid" "yellow")
         (circle 30 "solid" "red"))
(beside (circle 10 "solid" "red")
        (circle 20 "solid" "yellow")
        (circle 30 "solid" "red"))

(radial-star 5 20 64 "solid" "darkslategray")

;; Tile
(define YELLOWSQ (square 20 "solid" "yellow"))
(define BLUESQ (square 20 "solid" "blue"))

(define (tile sq0 sq1) (above (beside sq0 sq1)
                              (beside sq1 sq0)))
(tile YELLOWSQ BLUESQ)

;; compare images
(define IMAGE1 (rectangle 10 15 "solid" "red"))
(define IMAGE2 (rectangle 15 10 "solid" "red"))

IMAGE1
IMAGE2
(> (image-height IMAGE1) (image-height IMAGE2))
(< (image-width IMAGE1) (image-width IMAGE1))
(and (= (image-height IMAGE1) (image-height IMAGE2))
     (= (image-width IMAGE1) (image-width IMAGE2)))

;; max
(define (mymax a b) (if (> a b) a b))
(mymax 3 4)
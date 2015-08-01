(defun piglatin (word)
  (concatenate 'string (subseq word 1) (subseq word 0 1) "ay"))

(piglatin "pig")

; use #\ to represent char
; warning: member return rest of list when match
(defun isVowel (c) (member c '(#\a #\e #\i #\o #\u)))
(defun locVowel (str) (position-if #'isVowel str))
; reverse first vowel
(defun piglatin-1 (str)
  (let ((i (locVowel str)))
    (concatenate 'string
                 (subseq str i (+ i 1))
                 (subseq str 0 i)
                 (subseq str (+ i 1))
                 "ay")))

(isVowel #\e)
(isVowel #\n)
(locVowel "pig")
(locVowel "ant")

(piglatin-1 "ant")
(piglatin-1 "pig")
(piglatin-1 "fish")

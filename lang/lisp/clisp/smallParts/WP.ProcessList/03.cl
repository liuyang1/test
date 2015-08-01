; (map [result-type] [function] [seq])
(defun evenp-list (ls) (map 'list #'evenp ls))

(evenp-list '(1 2 3 4 5 6 7 8))

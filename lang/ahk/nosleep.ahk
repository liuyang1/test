; Ref: http://autohotkey.com/board/topic/93916-stay-awake/
; @robert_ilbrink
SetTimer, MoveMouse, 60000 ; Run MoveMouse every 1 minute

MoveMouse:
    MouseMove, 1, 0, 1, R  ;Move the mouse one pixel to the right
    MouseMove, -1, 0, 1, R ;Move the mouse back one pixel
return

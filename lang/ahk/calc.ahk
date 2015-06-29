#Persistent
#SingleInstance

run calc.exe
WinWaitActive, Calculator
; click 1
;MouseClick, left,  231, 259
; click +
;MouseClick, left, 348, 290
; click 2
;MouseClick, left, 270, 259
; click =
;MouseClick, left, 387, 259
; get 3

; instant click
SetControlDelay -1
; click 4
ControlClick, Button4
; click *
ControlClick, Button21
; click =
ControlClick, Button28

; FileSelctFile to open a dilag to select by user.
; quick script
ExitApp

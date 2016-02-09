#cs
   A set of small utilities for using within various AutoIT scripts.

   Written by Huy Nguyen (https://github.com/Florastamine)
#ce

#include-once
#include <Date.au3>
#include <FileConstants.au3>

Global Const $__FILE_NAME__ = "AU3assert.log"

Func assert($__cond, $__msg = "")
   if Not $__cond Then
	  Local Const $tfhandle = FileOpen($__FILE_NAME__, $FO_APPEND)
	  If Not ($tfhandle = -1) Then
		 Local Const $tsign = StringFormat("[%s, %s] Assertion failed: %s" & @CRLF, _NowDate(), _NowTime(), $__msg ? $__msg : "Undescribed assertion.")
		 FileWrite($tfHandle, $tsign)
		 FileClose($tfHandle)
	  EndIf
	  Exit
   EndIf
EndFunc

Func wait_window_active($__window)
   WinWaitActive($__window)
   Sleep(500)
EndFunc

Func close_current_window()
   Send("{LALT}+{SPACE}")
   Send("{C}")

   Return True
EndFunc


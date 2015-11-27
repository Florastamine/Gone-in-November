@echo off
cls
setlocal

set "RARPATH=C:\Program Files\WinRAR\"
set "DROPBOXPATH=D:\Dropbox"
set "DROPBOXCLIENT=C:\Program Files (x86)\Dropbox\Client"

rem Big thanks to Joey@stackoverflow for the neat trick for getting the 
rem illegal colons out of %time%! 
rem http://stackoverflow.com/questions/1642677/generate-unique-file-name-with-timestamp-in-batch-script

"%RARPATH%\rar" a %DROPBOXPATH%\GIN%time::=%.rar ..\*.* -r -t  -ai -k -rr10p -mt4 -m5 -ma4
"%DROPBOXCLIENT%\Dropbox.exe" /home

endlocal

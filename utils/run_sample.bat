@echo off
cls
setlocal
call ackvars.bat

%ACKPATH%\acknex __execsample.c

endlocal

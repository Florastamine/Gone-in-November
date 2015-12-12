@echo off
cls
setlocal
call ackvars.bat

%ACKPATH%\acknex ..\November.c -diag -nx 350 

endlocal

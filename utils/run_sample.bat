@echo off
setlocal
title Be patient...
cls
call ackvars.bat

echo Be patient...
%ACKPATH%\acknex -exe __execsample.c -nwnd

endlocal

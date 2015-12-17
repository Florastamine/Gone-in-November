@echo off
setlocal
title Be patient...
cls
call ackvars.bat

echo Be patient...
%ACKPATH%\acknex __execsample.c -nwnd

endlocal

@echo off
title 

rem Cleans up the cruft left by both Acknex and WED.

del /f /s /q ..\tests\acklog.txt
del /f /s /q ..\acklog.txt
del /f /s /q ..\scene*.$$M
del /f /s /q ..\scene\*.$$p
del /f /s /q ..\scene\*.pcx
del /f /s /q ..\scene\*.bak
del /f /s /q ..\scene\*.wed
del /f /s /q ..\scene\*.raw
del /f /s /q ..\scene\*.$$w

pause 

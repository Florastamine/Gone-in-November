@echo off
title 

rem Copies directories and subdirectories, including empty ones (/e), 
rem supress asking for overwritting (/y), and excludes the batch file itself
rem /EXCLUDE:list_file
rem %0: A built-in variable for determining the full path to the batch file
rem %~n0%~x0: Just fetch the batch filename only.

del /f /s /q                            D:\Dropbox\Public\DP\*.* >> log.log
xcopy /EXCLUDE:exclusion.excl /e /y     *.* D:\Dropbox\Public\DP\ >> log.log

pause

@echo off
setlocal
title 
cls

call ackvars.bat

echo Be patient...

%CLOC% ..\         >> cloc.log
%CLOC% --show-os   >> cloc.log
echo %time%      >> cloc.log
endlocal
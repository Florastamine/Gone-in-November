@echo off
setlocal
title 
cls

call ackvars.bat

echo Be patient...

%CLOC% ..\         >> cloc.log
%CLOC% --show-os --exclude-lang=XML   >> cloc.log
echo %time%      >> cloc.log
endlocal
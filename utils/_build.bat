@echo off
cls

rem Starts localization of environment variables in a batch file. Localization continues until a matching endlocal command is encountered or the end of the batch file is reached. (https://technet.microsoft.com/en-us/library/bb491001.aspx)
setlocal 

title Building in progress...

set "ACKPATH=H:\Development\Gamestudio\GStudio8\" 
set "path=%path%;%ACKPATH%" 
set start_time=%time%

rem ----- Begin the building phase -----

echo Create bindings
acknex __bind.c -nwnd -eq -nj -nv -wnd

echo Packing resources and creating executables
wed -r ..\\DPFL.c >> log.log

echo Cleaning up and copying leftovers

rem Cleans and moves build contents into the "builds" folder.
del /f ..\*.exe
del /f /s /q ..\builds\*.*
robocopy /move /e ..\DPFL.CD\ ..\builds

rem Copy remaining dependencies/files/etc.
copy %ACKPATH%\D3DCompiler_42.dll ..\builds\
copy %ACKPATH%\cudart32_41_22.dll ..\builds\

rem Copying streams & translations
mkdir ..\builds\sound\stream
copy ..\sound\stream\*.* ..\builds\sound\stream\

mkdir ..\builds\translation\
xcopy ..\translation\*.* ..\builds\translation\ /s /e /y

cls 
echo Waiting for user input to compile the WDF..
echo If the "Starter" box is unchecked, check it and press "OK".
rem Building and compiling the wdf. (manually)
wed -c ..\wdf\main.wdl
copy ..\wdf\main.cd\acknex.wdf ..\builds\
rd /s /q ..\wdf\main.cd\

rem ----- End of building -----

_cleanup

set end_time=%time%
set /a duration=%end_time%-%start_time%

echo Building started: %start_time%, completed: %end_time%.

pause
endlocal

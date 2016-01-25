@echo off
cls

title Building in progress...

rem Starts localization of environment variables in a batch file.
rem Localization continues until a matching endlocal command is 
rem encountered or the end of the batch file is reached. 
rem https://technet.microsoft.com/en-us/library/bb491001.aspx
setlocal

rem #include <ackvars>
call ackvars.bat

rem Set global variables
set "path=%path%;%ACKPATH%"
set start_time=%time%
set resources_exists=0

rem ----- Begin the building phase -----
echo Create bindings
acknex __bind.c -nwnd -eq -nj -nv -wnd

echo Packing resources and creating executables
rem (if we've not specified a resource file yet.)

IF NOT EXIST "%GAMEPATH%\*.wrs" (

echo Resource file not found. Will attempt to compile the resources...
wed -r ..\\November.c >> log.log

echo Cleaning up and copying leftovers

rem Cleans and copies build contents into the "builds" folder.
del /f /q ..\*.exe
del /f /s /q ..\builds\*.*

rem Robocopy suddenly stops working after a data lost... don't know why,
rem I switched to xcopy instead although it involves a full data-copy (double disk
rem storage) and I have to delete the original folder myself afterwards...
rem robocopy /copy /e /v ..\November.cd\ ..\builds

xcopy ..\November.cd\*.* ..\builds\ /s /e /y
del /f /s /q ..\November.cd\*.*
rd ..\November.cd

) ELSE (

echo Resources found at existing game path. Will only compile the executable.
rem (-exe: Compile to executable; -cc: Do not start the executable!)

set resources_exists=1
acknex -exe -cc ..\November.c

rem Copy the executable & resources afterwards
mkdir ..\builds\
move ..\*.exe ..\builds
move ..\*.dll ..\builds
copy "%GAMEPATH%\*.wrs" ..\builds

rem Copy remaining DLLs what are not automatically copied when using acknex -exe 
rem instead or wed -r...
copy "%ACKPATH%\acknex_plugins\ackphysX.dll" ..\builds\
copy "%ACKPATH%\d3dx9_42.dll" ..\builds\
copy "%ACKPATH%\d3dx9_30.dll" ..\builds\
copy "%ACKPATH%\Nx*.dll" ..\builds\
copy "%ACKPATH%\PhysX*.dll" ..\builds\

rem F_cking WED can't take a simple relative path or accept wildcards?
rem wed -p ..\builds\november.exe
wed -p "%REPOPATH%\builds\november.exe"
)

rem Copy remaining dependencies/files/etc.
copy "%ACKPATH%\D3DCompiler_42.dll" ..\builds\
copy "%ACKPATH%\cudart32_41_22.dll" ..\builds\
copy "%ACKPATH%\d3dx9_30.dll" ..\builds\

rem Compress the GiN executable
IF EXIST "%UPXPATH%\upx.exe" (
echo Performing UPX compression...
%UPXPATH%\upx -9 -f ..\builds\November.exe
)

mkdir ..\builds\redist
xcopy .\redist\*.* ..\builds\redist\ /s /e /y

rem Copying streams, translations and configuration files
mkdir ..\builds\sound\stream
copy ..\sound\stream\*.* ..\builds\sound\stream\

mkdir ..\builds\translation\
xcopy ..\translation\*.* ..\builds\translation\ /s /e /y

mkdir ..\builds\cfg\
copy ..\cfg\*.cfg ..\builds\cfg\

mkdir ..\builds\save\
copy ..\save\*.sav ..\builds\save\

copy %ACKPATH%\acknex_plugins\*.dll ..\builds\

rem Icon
copy 32.ico ..\builds\

cls
echo Waiting for user input to compile the WDF..
echo If the "Starter" box is unchecked, check it and press "OK".
rem Building and compiling the wdf. (manually)
IF EXIST "..\wdf\acknex.wdf" (
copy ..\wdf\acknex.wdf ..\builds\
) ELSE (
wed -c ..\wdf\main.wdl
copy ..\wdf\acknex.wdf ..\builds\
rd /s /q ..\wdf\main.cd\
)

cls
rem Final cleanup & polishing
echo Cleaning up...

IF EXIST "..\builds\acknex.dll" (
rename "..\builds\acknex.dll" "Kernel.dll"
)

del /f /s /q ..\builds\*.md
cleanup

cls
rem ----- End of building -----

set end_time=%time%
set /a duration=%end_time%-%start_time%

cls
echo Building started: %start_time%, completed: %end_time%.

pause
endlocal

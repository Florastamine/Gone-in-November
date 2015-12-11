@echo off
setlocal
cls

title Be patient...

rem Query the architecture of Windows, in a very simple way.
IF EXIST "%PROGRAMFILES(X86)%" (
SET "TPATH=%WINDIR%\SysWOW64"
echo 64-bit Windows detected.
) ELSE (
SET "TPATH=%WINDIR%\System32"
echo 32-bit Windows detected.
)

rem Removing the OGG DLLs.
regsvr32 %TPATH%\OggDS.dll /u /s
del /f %TPATH%\ogg.dll %TPATH%\vorbis.dll %TPATH%\vorbisenc.dll %TPATH%\OggDS.dll

endlocal
pause

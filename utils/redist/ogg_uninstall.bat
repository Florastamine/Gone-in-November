@echo off
setlocal
cls

title Be patient...

>nul 2>&1 "%SYSTEMROOT%\system32\cacls.exe" "%SYSTEMROOT%\system32\config\system"
if '%errorlevel%' NEQ '0' (
    echo Requesting administrative privileges...
    goto UACPrompt
) else ( goto req_success )

:UACPrompt
    echo Set UAC = CreateObject^("Shell.Application"^) > "%temp%\getadmin.vbs"
    set params = %*:"="
    echo UAC.ShellExecute "cmd.exe", "/c %~s0 %params%", "", "runas", 1 >> "%temp%\getadmin.vbs"

    "%temp%\getadmin.vbs"
    del "%temp%\getadmin.vbs"
    exit /B

:req_success
    pushd "%CD%"
    CD /D "%~dp0"

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

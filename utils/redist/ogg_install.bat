@echo off
title Be patient...
setlocal
cls

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

rem Now installing and registering the OGG DLLs.
rem %~dp0 points to the path to the currently executed batch script.
rem Using copy *.dll only isn't work - and it's my fault (Solution discussed in superuser here):
rem http://superuser.com/questions/844507/copying-files-in-windows-7-command-line-the-system-cannot-find-the-file-specif

copy %~dp0\*.dll %TPATH%\
regsvr32 %TPATH%\OggDS.dll /s

endlocal

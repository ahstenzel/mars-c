:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
::    __  __       __     _____   _____
::   /  \/  \     /  \   |     \ /
::  /        \   /----\  |-----/ \-----\
:: /          \ /      \ |     \  _____/
::
:: Installs library files and headers to a dedicated directory in 
:: Program Files. Defaults to installing library files for the host
:: architecture. {TODO: Register DLLs}
::
:: build_win.bat [options]
::   /A {value}: Set target architecture. Defaults to host architecture.
::       x86:   32-bit native
::       x64:   64-bit native
::       arm:   32-bit ARM
::       arm64: 64-bit ARM
::   /N: Do not wait for user input when install is finished
::    
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
@echo OFF

:: BatchGotAdmin
:-------------------------------------
REM  --> Check for permissions
    IF "%PROCESSOR_ARCHITECTURE%" EQU "amd64" (
>nul 2>&1 "%SYSTEMROOT%\SysWOW64\cacls.exe" "%SYSTEMROOT%\SysWOW64\config\system"
) ELSE (
>nul 2>&1 "%SYSTEMROOT%\system32\cacls.exe" "%SYSTEMROOT%\system32\config\system"
)

REM --> If error flag set, we do not have admin.
if '%errorlevel%' NEQ '0' (
    echo Requesting administrative privileges...
    goto UACPrompt
) else ( goto gotAdmin )

:UACPrompt
    echo Set UAC = CreateObject^("Shell.Application"^) > "%temp%\getadmin.vbs"
    set params= %*
    echo UAC.ShellExecute "cmd.exe", "/c ""%~s0"" %params:"=""%", "", "runas", 1 >> "%temp%\getadmin.vbs"

    "%temp%\getadmin.vbs"
    del "%temp%\getadmin.vbs"
    exit /B

:gotAdmin
    pushd "%CD%"
    CD /D "%~dp0"
:-------------------------------------- 

:: Define macros
reg Query "HKLM\Hardware\Description\System\CentralProcessor\0" | find /i "x86" > NUL && set HOST=x86 || set HOST=x64
set CPATH=%~dp0

:: Parse arguments
set TARGET=%HOST%
set NOWAIT=0
:Args
set arg=%1
if defined arg (
	if "%1"=="/A" (
		:: Get architecture
		set TARGET=%2
		shift
	) else if "%1"=="/N" (
		:: Set No Wait
		set NOWAIT=1
	) else (
		goto ShowHelp
	)
	shift
	goto Args
)

:: Check for valid environment
if "%TARGET%"=="x86" goto Install
if "%TARGET%"=="x64" goto Install
if "%TARGET%"=="arm" goto Install
if "%TARGET%"=="arm64" goto Install
goto ShowHelp

:: Move files
:Install
echo Installing...
set INC_DIR=%CPATH%..\include
set BIN_DIR=%CPATH%..\bin\win\%TARGET%
set OUT_DIR=C:\Program Files\Mars
if not exist "%OUT_DIR%\lib\%TARGET%" mkdir "%OUT_DIR%\lib\%TARGET%"
if not exist "%OUT_DIR%\include" mkdir "%OUT_DIR%\include"
copy "%BIN_DIR%\*" "%OUT_DIR%\lib\%TARGET%"
copy "%INC_DIR%\*" "%OUT_DIR%\include"
goto End

:: Show syntax
:ShowHelp
echo install_win.bat [options]
echo  /A: Architecture {x86, x86, arm, arm64} (Default: host architecture)
echo  /N: Do not wait for user input after process is done

:End
if %NOWAIT%==0 pause
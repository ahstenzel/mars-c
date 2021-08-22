:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
::    __  __       __     _____   _____
::   /  \/  \     /  \   |     \ /
::  /        \   /----\  |-----/ \-----\
:: /          \ /      \ |     \  _____/
::
:: Builds and installs library files on a Windows host platform using the 
:: MSVC toolchain. Defaults to building for the host architecture.
::
:: build_win.bat [options]
::   /A {value}: Set target architecture. Defaults to host architecture.
::       x86:   32-bit native
::       x64:   64-bit native
::       arm:   32-bit ARM
::       arm64: 64-bit ARM
::   /Db: Build with debug tools
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
set VCVARS="C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat"
reg Query "HKLM\Hardware\Description\System\CentralProcessor\0" | find /i "x86" > NUL && set HOST=x86 || set HOST=x64
set CPATH=%~dp0

:: Parse arguments
set TARGET=%HOST%
set CONFIG=release
set NOWAIT=0
:Args
set arg=%1
if defined arg (
	if "%1"=="/A" (
		:: Get architecture
		set TARGET=%2
		shift
	) else if "%1"=="/Db" (
		:: Set debug config
		set CONFIG=debug
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
if "%TARGET%"=="x86" goto LoadEnv
if "%TARGET%"=="x64" goto LoadEnv
if "%TARGET%"=="arm" goto LoadEnv
if "%TARGET%"=="arm64" goto LoadEnv
goto ShowHelp

:: Load correct environment
:LoadEnv
set FULLTARGET=%TARGET%
if not "%TARGET%"=="%HOST%" set FULLTARGET=%HOST%_%TARGET%
echo Loading MSVC %FULLTARGET% environment...
if not defined DevEnvDir (
	call %VCVARS% %FULLTARGET%
)

:: Check tools loaded correctly
if not defined DevEnvDir (
	echo Failed to load Visual Studio development environment!
	goto End
)

:: Set environment variables
set /p VER=<%CPATH%..\VERSION
set OUT=libmars
set CFLAGS=/DVERSION=%VER%
set INC_DIR=%CPATH%..\include
set SRC_DIR=%CPATH%..\src
set BIN_DIR=%CPATH%..\bin\win\%TARGET%
set OUT_DIR=C:\Program Files\Mars
if "%CONFIG%"=="debug" (
	set CFLAGS=%CFLAGS% /DDEBUG
	set OUT=%OUT%d
) else (
	set CFLAGS=%CFLAGS% /O2 /EHsc
)

:: Run build sequence
echo Running build sequence...
if not exist %BIN_DIR% mkdir %BIN_DIR%
cl.exe /I%INC_DIR% %CFLAGS% /LD /Fo%CPATH%%OUT%.o %SRC_DIR%\libmars.c
if not %ERRORLEVEL% == 0 (
	echo Build failed!
	goto End
)
if exist %CPATH%%OUT%.o del %CPATH%%OUT%.o
if exist %CPATH%%OUT%.dll move %CPATH%%OUT%.* %BIN_DIR%

:: Install
echo Installing...
if not exist "%OUT_DIR%\lib\%TARGET%" mkdir "%OUT_DIR%\lib\%TARGET%"
if not exist "%OUT_DIR%\include" mkdir "%OUT_DIR%\include"
copy "%BIN_DIR%\*" "%OUT_DIR%\lib\%TARGET%"
copy "%INC_DIR%\*" "%OUT_DIR%\include"
goto End

:: Show syntax
:ShowHelp
echo build_win.bat [options]
echo  /A: Architecture {x86, x86, arm, arm64} (Default: host architecture)
echo  /Db: Build with debug tools
echo  /N: Do not wait for user input after process is done

:End
if %NOWAIT%==0 pause
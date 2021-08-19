:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
::    __  __       __     _____   _____
::   /  \/  \     /  \   |     \ /
::  /        \   /----\  |-----/ \-----\
:: /          \ /      \ |     \  _____/
::
:: Build a library file on a Windows host platform using x86 host build tools.
:: Must supply the target architecture, defaults to a release configuration
:: and a static library file.
::
:: build_win.bat [options]
::   /A {value}: Set architecture to {value}. Possible values:
::               x86:   32-bit native (Default)
::               x64:   64-bit native
::               arm:   32-bit ARM
::               arm64: 64-bit ARM
::   /Db: Build with debug functionality
::   /Dl: Build as DLL
::    
:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::
@echo OFF
set vcvars="C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat"
set cpath=%~dp0

:: Parse arguments
set arch=x86
set config=release
set linkage=static
:Args
set arg=%1
if defined arg (
	:: Get architecture
	if "%1"=="/A" (
		set arch=%2
		shift
	)
	:: Set debug config
	if "%1"=="/Db" (
		set config=debug
	)
	:: Set dynamic linking
	if "%1"=="/Dl" (
		set linkage=dynamic
	)
	shift
	goto Args
)

:: Check for valid environment
if "%arch%"=="x86" goto LoadEnv
if "%arch%"=="x64" goto LoadEnv
if "%arch%"=="arm" goto LoadEnv
if "%arch%"=="arm64" goto LoadEnv
goto ShowHelp

:: Load correct environment
:LoadEnv
set fullarch=%arch%
if not "%arch%"=="x86" set fullarch=x86_%fullarch%
echo Loading Windows %arch% environment...
if not defined DevEnvDir (
	call %vcvars% %fullarch%
)

:: Check tools loaded correctly
if not defined DevEnvDir (
	echo Failed to load Visual Studio development environment!
	goto End
)

:: Run make sequence
echo Running makefile...
set flags=ARCH="%arch%" DIR_ROOT=%cpath%..
if "%config%"=="debug" (
	set flags=%flags% CFLAGS="/c /DDEBUG" OUT="libmarsd"
)
if "%linkage%"=="dynamic" (
	set flags=%flags% LINKER="link.exe" LFLAGS="/dll /out:$(DIR_BIN)\$(OUT).dll"
)
nmake /F %cpath%Makefile_win all %flags%
nmake /F %cpath%Makefile_win clean
goto End

:: Show syntax
:ShowHelp
echo build_win.bat [options]
echo  /A: Architecture {[x86], x86, arm, arm64}
echo  /Db: Debug mode
echo  /Dl: Build dynamic library

:End
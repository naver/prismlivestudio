REM ****************************************************************
rem autobuild.cmd parameter
rem %1 [rebuild] rebuild | build   , control build range
rem %2 [Release] | Debug | Release     , control build config
rem %3 [x64] | Win32 | x64     , control build arch
rem %4 [remote] local | remote , the build is used in local or CI server
rem %5 [develop] , the branch will be checkout, default is [develop]
REM ****************************************************************

@echo -----------------autobuild.cmd-----------------


@echo -----------------start build-----------------
if "%QTDIR32%"=="" goto ERROR
if "%QTDIR64%"=="" goto ERROR

rem set build default option
set BUILD_RANGE=rebuild
set BUILD_CONFIG=Release
set MULTI_ARCH=x64

if  not "%1" == ""  set BUILD_RANGE=%1
if  not "%2" == ""  set BUILD_CONFIG=%2
if  not "%3" == ""  set MULTI_ARCH=%3

set rootPath=..\..\

cd %rootPath%
if not "%ERRORLEVEL%" == "0" goto ERROR

rmdir .\bin /s /q
if not "%ERRORLEVEL%" == "0" goto ERROR

if not exist .\bin mkdir .\bin

cd build\windows


if "%MULTI_ARCH%" == "x64"  goto BuildX64

:BuildWin32

@echo -----------------start build 32-----------------

call configure.cmd
call build.cmd

if "%MULTI_ARCH%" == "Win32"  goto BUILDEND


:BuildX64

@echo -----------------start build 64-----------------

call configure.cmd
call build.cmd

:BUILDEND

@echo -----------------build and deploy finished-----------------
goto OK

:ERROR
@echo build failed
@exit /b 1

:OK

@echo build success

:END

@exit /b 0

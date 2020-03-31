@echo off

setlocal

set PROJECT_DIR=%~dp0..\..\
set BIN_DIR=%PROJECT_DIR%\bin
set SRC_DIR=%PROJECT_DIR%\src
set OBS_DIR=%SRC_DIR%\obs
set PRISM_DIR=%SRC_DIR%\prism
set OBS_SRC_DIR=%OBS_DIR%
set PRISM_SRC_DIR=%PRISM_DIR%
set OBS_BUILD_DIR=%OBS_DIR%\build
set PRISM_BUILD_DIR=%PRISM_DIR%\build
set NOT "%1"=="" set MULTI_ARCH=%1

if "%BUILD_CONFIG%"=="Debug" (
	set BUILD_TYPE=Debug
) else (
	set BUILD_TYPE=RelWithDebInfo
)

if NOT "%MULTI_ARCH%"=="" (
    set ARCH=%MULTI_ARCH%
) else (
    if "%ARCH%"=="" set ARCH=x64
)

if "%BUILD_RANGE%"=="rebuild" set CLEAN_FIRST=--clean-first
set PRISM_BUILD_ARCH_DIR=%PRISM_BUILD_DIR%
if NOT "%MULTI_ARCH%"=="" set PRISM_BUILD_ARCH_DIR=%PRISM_BUILD_ARCH_DIR%\%MULTI_ARCH%

echo clear previous build rundir.
set OBS_BUILD_RUNDIR=%OBS_BUILD_DIR%\rundir
if EXIST "%OBS_BUILD_RUNDIR%" cmake -E remove_directory "%OBS_BUILD_RUNDIR%"
set PRISM_BUILD_RUNDIR=%PRISM_BUILD_DIR%\rundir
if EXIST "%PRISM_BUILD_RUNDIR%" cmake -E remove_directory "%PRISM_BUILD_RUNDIR%"

echo build PRISMLiveStudio start.

echo cmake --build "%PRISM_BUILD_ARCH_DIR%" --target ALL_BUILD --config %BUILD_TYPE% %CLEAN_FIRST%
cmake --build "%PRISM_BUILD_ARCH_DIR%" --target ALL_BUILD --config %BUILD_TYPE% %CLEAN_FIRST%

echo build PRISMLiveStudio complete.

@echo off

setlocal

set OBS_BUILD_DIR=%1
set CEF_ROOT_DIR=%2
if "%BUILD_TYPE%"=="" set BUILD_TYPE=%3
if "%BUILD_RANGE%"=="rebuild" set CLEAN_FIRST=--clean-first

set OBS_BUILD_ARCH_DIR=%OBS_BUILD_DIR%
if NOT "%MULTI_ARCH%"=="" set OBS_BUILD_ARCH_DIR=%OBS_BUILD_ARCH_DIR%\%MULTI_ARCH%
echo obs build.
echo ******************************************************************************
echo cmake --build "%OBS_BUILD_ARCH_DIR%" --target ALL_BUILD --config %BUILD_TYPE% %CLEAN_FIRST%
echo ******************************************************************************
cmake --build "%OBS_BUILD_ARCH_DIR%" --target ALL_BUILD --config %BUILD_TYPE% %CLEAN_FIRST%

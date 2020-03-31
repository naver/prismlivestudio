@echo off

setlocal

set OBS_DIR=%1
set PRISM_DIR=%2
set OUTPUT_BIN_DIR=%3
set BUILD_TYPE=%4

set OBS_BUILD_DIR=%OBS_DIR%\build
set PRISM_BUILD_DIR=%PRISM_DIR%\build

set OBS_BUILD_ARCH_DIR=%OBS_BUILD_DIR%
if NOT "%MULTI_ARCH%"=="" set OBS_BUILD_ARCH_DIR=%OBS_BUILD_ARCH_DIR%\%MULTI_ARCH%

if "%ARCH%"=="Win32" (
  set SUFFIX=32bit
) else (
  set SUFFIX=64bit
)

if NOT EXIST "%OUTPUT_BIN_DIR%" mkdir "%OUTPUT_BIN_DIR%"
if NOT EXIST "%OUTPUT_BIN_DIR%\data" mkdir "%OUTPUT_BIN_DIR%\data"
if NOT EXIST "%OUTPUT_BIN_DIR%\data\libobs" mkdir "%OUTPUT_BIN_DIR%\data\libobs"
if NOT EXIST "%OUTPUT_BIN_DIR%\data\prism-plugins" mkdir "%OUTPUT_BIN_DIR%\data\prism-plugins"
if NOT EXIST "%OUTPUT_BIN_DIR%\prism-plugins" mkdir "%OUTPUT_BIN_DIR%\prism-plugins"

chdir %PRISM_DIR%
if EXIST "%OBS_BUILD_ARCH_DIR%\rundir\%BUILD_TYPE%\bin\%SUFFIX%" xcopy /D /E /C /I /F /R /Y /EXCLUDE:not-copy-bins.txt "%OBS_BUILD_ARCH_DIR%\rundir\%BUILD_TYPE%\bin\%SUFFIX%" "%OUTPUT_BIN_DIR%"
if EXIST "%OBS_BUILD_ARCH_DIR%\rundir\%BUILD_TYPE%\data\libobs" xcopy /D /E /C /I /F /R /Y "%OBS_BUILD_ARCH_DIR%\rundir\%BUILD_TYPE%\data\libobs" "%OUTPUT_BIN_DIR%\data\libobs"
if EXIST "%OBS_BUILD_ARCH_DIR%\rundir\%BUILD_TYPE%\data\obs-plugins" xcopy /D /E /C /I /F /R /Y "%OBS_BUILD_ARCH_DIR%\rundir\%BUILD_TYPE%\data\obs-plugins" "%OUTPUT_BIN_DIR%\data\prism-plugins"
if EXIST "%OBS_BUILD_ARCH_DIR%\rundir\%BUILD_TYPE%\data\obs-scripting\%SUFFIX%" xcopy /D /E /C /I /F /R /Y "%OBS_BUILD_ARCH_DIR%\rundir\%BUILD_TYPE%\data\obs-scripting\%SUFFIX%" "%OUTPUT_BIN_DIR%\prism-plugins\obs-scripting"
if EXIST "%OBS_BUILD_ARCH_DIR%\rundir\%BUILD_TYPE%\obs-plugins\%SUFFIX%" xcopy /D /E /C /I /F /R /Y "%OBS_BUILD_ARCH_DIR%\rundir\%BUILD_TYPE%\obs-plugins\%SUFFIX%" "%OUTPUT_BIN_DIR%\prism-plugins"
if EXIST "%OBS_DIR%\plugins\win-capture\game-bin" xcopy /D /E /C /I /F /R /Y "%OBS_DIR%\plugins\win-capture\game-bin" "%OUTPUT_BIN_DIR%\data\prism-plugins\win-capture"

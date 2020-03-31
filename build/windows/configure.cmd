@echo off

setlocal

set PROJECT_DIR=%~dp0..\..
set BIN_DIR=%PROJECT_DIR%\bin
set SRC_DIR=%PROJECT_DIR%\src
set OBS_DIR=%SRC_DIR%\obs
set PRISM_DIR=%SRC_DIR%\prism
set OBS_SRC_DIR=%OBS_DIR%
set PRISM_SRC_DIR=%PRISM_DIR%
set OBS_BUILD_DIR=%OBS_DIR%\build
set PRISM_BUILD_DIR=%PRISM_DIR%\build
set /p PRISM_VER=<%~dp0version_win.txt

rem set DEBUG_API=DEBUG_API
set NOT "%1"=="" set MULTI_ARCH=%1

if "%QTDIR32%"=="" set QTDIR32=C:\Qt\Qt5.12.6\5.12.6\msvc2017
if "%QTDIR64%"=="" set QTDIR64=C:\Qt\Qt5.12.6\5.12.6\msvc2017_64
rem ARCH=Win32/x64
if NOT "%MULTI_ARCH%"=="" (
    set ARCH=%MULTI_ARCH%
) else (
    if "%ARCH%"=="" set ARCH=x64
)
rem BUILD_TYPE=Debug/Release
if "%BUILD_CONFIG%"=="Debug" (
	set BUILD_TYPE=Debug
) else (
	set BUILD_TYPE=RelWithDebInfo
)


@echo %QTDIR32%
@echo %QTDIR64%
@echo %ARCH%
@echo %BUILD_TYPE%
@echo %BUILD_RANGE%
if "%GENERATOR_VS%"=="" (
    set GENERATOR=Visual Studio 16 2019
) else (
    set GENERATOR=%GENERATOR_VS%
)
rem set TOOLSET=v141
if "%VERSION%"=="" (
    if "%PRISM_VER%"=="" (
        set RELEASE_CANDIDATE = 2.0.0.0
    ) else (
        set RELEASE_CANDIDATE=%PRISM_VER%
    )
) else (
    set RELEASE_CANDIDATE=%VERSION%
)

if NOT "%ARCH%"=="Win32" (
    if NOT "%ARCH%"=="x64" (
        set ARCH=x64
    )
)

if "%ARCH%"=="Win32" (
    set QTDIR=%QTDIR32%
    set CEF_ROOT_DIR=%OBS_DIR%\deps\cef\win32
) else (
    set QTDIR=%QTDIR64%
    set CEF_ROOT_DIR=%OBS_DIR%\deps\cef\win64
)

set OUTPUT_BIN_DIR=%BIN_DIR%\%BUILD_TYPE%\%ARCH%
set PRISM_BUILD_ARCH_DIR=%PRISM_BUILD_DIR%
if NOT "%MULTI_ARCH%"=="" set PRISM_BUILD_ARCH_DIR=%PRISM_BUILD_ARCH_DIR%\%MULTI_ARCH%

if EXIST "%QTDIR%" (
    if NOT "%ARCH%"=="" (
        echo cmake -S "%PRISM_SRC_DIR%" -B "%PRISM_BUILD_ARCH_DIR%" -G "%GENERATOR%" -A %ARCH% -DCMAKE_BUILD_TYPE=%BUILD_TYPE% -DCMAKE_PREFIX_PATH="%QTDIR%" -DRELEASE_CANDIDATE=%RELEASE_CANDIDATE% -DBROWSER_AVAILABLE_INTERNAL="ON"
        cmake -S "%PRISM_SRC_DIR%" -B "%PRISM_BUILD_ARCH_DIR%" -G "%GENERATOR%" -A %ARCH% -DCMAKE_BUILD_TYPE=%BUILD_TYPE% -DCMAKE_PREFIX_PATH="%QTDIR%" -DRELEASE_CANDIDATE=%RELEASE_CANDIDATE% -DBROWSER_AVAILABLE_INTERNAL="ON"
    ) else (
        echo Missing ARCH.
    )
) else (
    echo Missing QTDIR.
)


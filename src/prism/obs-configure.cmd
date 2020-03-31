@echo off

setlocal

set DepsPath32=%OBS_DIR%\dependencies\win32
set DepsPath64=%OBS_DIR%\dependencies\win64

set CEF_BUILD_DIR=%CEF_ROOT_DIR%\build
echo cef configure.
echo ******************************************************************************
echo cmake -Wno-dev -S "%CEF_ROOT_DIR%" -B "%CEF_BUILD_DIR%" -G "%GENERATOR%" -A %ARCH% -DCMAKE_BUILD_TYPE=%BUILD_TYPE%
echo ******************************************************************************
cmake -Wno-dev -S "%CEF_ROOT_DIR%" -B "%CEF_BUILD_DIR%" -G "%GENERATOR%" -A %ARCH% -DCMAKE_BUILD_TYPE=%BUILD_TYPE%
cmake --build "%CEF_BUILD_DIR%" --target ALL_BUILD --config Debug %CLEAN_FIRST%
cmake --build "%CEF_BUILD_DIR%" --target ALL_BUILD --config Release %CLEAN_FIRST%

set OBS_BUILD_ARCH_DIR=%OBS_BUILD_DIR%
if NOT "%MULTI_ARCH%"=="" set OBS_BUILD_ARCH_DIR=%OBS_BUILD_ARCH_DIR%\%MULTI_ARCH%
echo obs configure.
echo ******************************************************************************
echo cmake -Wno-dev -S "%OBS_SRC_DIR%" -B "%OBS_BUILD_ARCH_DIR%" -G "%GENERATOR%" -A %ARCH% -DCMAKE_BUILD_TYPE=%BUILD_TYPE% -DCMAKE_PREFIX_PATH="%QTDIR%" -DRELEASE_CANDIDATE=%RELEASE_CANDIDATE% -DCOPIED_DEPENDENCIES=false -DCOPY_DEPENDENCIES=true -DBUILD_CAPTIONS=true -DCOMPILE_D3D12_HOOK=true -DBUILD_BROWSER=true -DCEF_ROOT_DIR=%CEF_ROOT_DIR%
echo ******************************************************************************
cmake -Wno-dev -S "%OBS_SRC_DIR%" -B "%OBS_BUILD_ARCH_DIR%" -G "%GENERATOR%" -A %ARCH% -DCMAKE_BUILD_TYPE=%BUILD_TYPE% -DCMAKE_PREFIX_PATH="%QTDIR%" -DRELEASE_CANDIDATE=%RELEASE_CANDIDATE% -DCOPIED_DEPENDENCIES=false -DCOPY_DEPENDENCIES=true -DBUILD_CAPTIONS=true -DCOMPILE_D3D12_HOOK=true -DBUILD_BROWSER=true -DCEF_ROOT_DIR=%CEF_ROOT_DIR%

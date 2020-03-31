cmake_minimum_required(VERSION 3.1.0)

project(PRISMLiveStudio)

set(DepsPath32 $ENV{OBS_DIR}/dependencies/win32/include)
set(DepsPath64 $ENV{OBS_DIR}/dependencies/win64/include)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "$ENV{PRISM_DIR}/cmake/Modules" "$ENV{OBS_DIR}/cmake/Modules")

include(ObsHelpers)
include(ObsCpack)
include(GNUInstallDirs)

# Must be a string in the format of "x.x.x-rcx"
if(DEFINED RELEASE_CANDIDATE)
	set(PLS_VERSION "${RELEASE_CANDIDATE}")
	string(REPLACE "-rc" "." RC_SPLIT ${RELEASE_CANDIDATE})
	string(REPLACE "." ";" RC_SPLIT ${RC_SPLIT})
	message(WARNING "******************************************************************************\nRelease candidate deteced, PLS_VERSION is now: ${PLS_VERSION}\n******************************************************************************")
	list(GET RC_SPLIT 0 PLS_RELEASE_CANDIDATE_MAJOR)
	list(GET RC_SPLIT 1 PLS_RELEASE_CANDIDATE_MINOR)
	list(GET RC_SPLIT 2 PLS_RELEASE_CANDIDATE_PATCH)
	list(GET RC_SPLIT 3 PLS_RELEASE_CANDIDATE)
else()
	set(PLS_RELEASE_CANDIDATE_MAJOR 0)
	set(PLS_RELEASE_CANDIDATE_MINOR 0)
	set(PLS_RELEASE_CANDIDATE_PATCH 0)
	set(PLS_RELEASE_CANDIDATE 0)
endif()

# Binary Versioning for Windows
if(WIN32)
	string(REPLACE "-" ";" UI_VERSION_SPLIT ${PLS_VERSION})
	list(GET UI_VERSION_SPLIT 0 UI_VERSION)
	string(REPLACE "." ";" UI_VERSION_SEMANTIC ${UI_VERSION})
	list(GET UI_VERSION_SEMANTIC 0 UI_VERSION_MAJOR)
	list(GET UI_VERSION_SEMANTIC 1 UI_VERSION_MINOR)
	list(GET UI_VERSION_SEMANTIC 2 UI_VERSION_PATCH)
endif()

function(copy_prism_target target)
    add_custom_command(TARGET ${target}
	    POST_BUILD
	    COMMAND "${CMAKE_COMMAND}"
		    "-DCONFIG=$<CONFIGURATION>"
		    "-DPRISM_DIR=$ENV{PRISM_DIR}"
            "-DSRC_DIR=${CMAKE_CURRENT_SOURCE_DIR}"
		    "-DBIN_DIR=$ENV{BIN_DIR}"
		    "-DARCH=$ENV{ARCH}"
            "-DTARGET=${target}"
		    -P "$ENV{PRISM_DIR}/cmake/copy_prism_target.cmake"
	    VERBATIM)
endfunction()

function(copy_prism_plugin_target target)
    add_custom_command(TARGET ${target}
	    POST_BUILD
	    COMMAND "${CMAKE_COMMAND}"
		    "-DCONFIG=$<CONFIGURATION>"
		    "-DPRISM_DIR=$ENV{PRISM_DIR}"
            "-DSRC_DIR=${CMAKE_CURRENT_SOURCE_DIR}"
		    "-DBIN_DIR=$ENV{BIN_DIR}"
		    "-DARCH=$ENV{ARCH}"
            "-DTARGET=${target}"
		    -P "$ENV{PRISM_DIR}/cmake/copy_prism_plugin_target.cmake"
	    VERBATIM)
endfunction()

function(copy_prism_theme_target target)
    add_custom_command(TARGET ${target}
	    POST_BUILD
	    COMMAND "${CMAKE_COMMAND}"
		    "-DCONFIG=$<CONFIGURATION>"
		    "-DPRISM_DIR=$ENV{PRISM_DIR}"
		    "-DSRC_DIR=${CMAKE_CURRENT_SOURCE_DIR}"
		    "-DBIN_DIR=$ENV{BIN_DIR}"
		    "-DARCH=$ENV{ARCH}"
		    "-DTARGET=${target}"
		    -P "$ENV{PRISM_DIR}/cmake/copy_prism_theme_target.cmake"
	    VERBATIM)
endfunction()

function(add_obs_dependency target)
    add_dependencies(${target} obs-build obs-copy)
endfunction()

function(link_obs target)
    target_link_libraries(${target} obs)
endfunction()

function(link_jansson target)
    target_link_libraries(${target} debug jansson_d optimized jansson)
endfunction()

function(link_nelo target)
    target_link_libraries(${target}
	debug "$ENV{PRISM_DIR}/main/deps/nelo/lib/Debug/$ENV{ARCH}/libnelo2d.lib"
	optimized "$ENV{PRISM_DIR}/main/deps/nelo/lib/Release/$ENV{ARCH}/libnelo2.lib")
endfunction()

function(link_hmac target)
    target_link_libraries(${target}
	debug "$ENV{PRISM_DIR}/libs/HMAC/lib/Debug/$ENV{ARCH}/libApiGateway-MAC.lib"
	optimized "$ENV{PRISM_DIR}/libs/HMAC/lib/Release/$ENV{ARCH}/libApiGateway-MAC.lib")
endfunction()

execute_process(COMMAND $ENV{ComSpec} /c ${CMAKE_CURRENT_SOURCE_DIR}/obs-configure)
# Modify obs-scripting plugin path
set(SCRIPTING_ENABLED ON)
set(OBS_SCRIPT_PLUGIN_PATH prism-plugins/obs-scripting)
set(PYTHON_LIB python36)
set(COMPILE_LUA TRUE)
set(LUAJIT_FOUND TRUE)
set(COMPILE_PYTHON TRUE)
set(PYTHON_FOUND TRUE)
set(UI_ENABLED TRUE)
set(BUILD_CAPTIONS TRUE)
configure_file(
    "$ENV{OBS_DIR}/deps/obs-scripting/obs-scripting-config.h.in"
    "$ENV{OBS_BUILD_DIR}/$ENV{MULTI_ARCH}/deps/obs-scripting/obs-scripting-config.h")
if(MSVC)
  file(GENERATE
    OUTPUT "$ENV{OBS_BUILD_DIR}/$ENV{MULTI_ARCH}/ALL_BUILD.vcxproj.user"
    INPUT "$ENV{PRISM_DIR}/cmake/ALL_BUILD.vcxproj.user.in")
endif()

include_directories(
	"$ENV{OBS_BUILD_DIR}/$ENV{MULTI_ARCH}/config"
	"$ENV{OBS_DIR}/libobs"
	"$ENV{OBS_DIR}/deps/libff"
	"$ENV{OBS_DIR}/deps/json11"
	"$ENV{OBS_DIR}/UI/obs-frontend-api"
	"$ENV{OBS_BUILD_DIR}/$ENV{MULTI_ARCH}/deps/jansson/include"
	"$ENV{OBS_DIR}/deps/blake2/src"
	"$ENV{PRISM_DIR}/main/deps/nelo/include"
	"$ENV{PRISM_DIR}/main/frontend-api"
	"$ENV{PRISM_DIR}/common"
	"$ENV{PRISM_DIR}/common/liblog/liblog"
	"$ENV{PRISM_DIR}/common/liblog/log")
link_directories(
	"$ENV{OBS_BUILD_DIR}/$ENV{MULTI_ARCH}/libobs"
	"$ENV{OBS_BUILD_DIR}/$ENV{MULTI_ARCH}/deps/jansson/lib"
	"$ENV{OBS_BUILD_DIR}/$ENV{MULTI_ARCH}/deps/blake2"
	"$ENV{OBS_BUILD_DIR}/$ENV{MULTI_ARCH}/deps/w32-pthreads"
	"$ENV{OBS_BUILD_DIR}/$ENV{MULTI_ARCH}/UI/obs-frontend-api"
	"$ENV{PRISM_BUILD_DIR}/$ENV{MULTI_ARCH}/main/frontend-api"
	"$ENV{PRISM_BUILD_DIR}/$ENV{MULTI_ARCH}/common/liblog/liblog"
	"$ENV{PRISM_BUILD_DIR}/$ENV{MULTI_ARCH}/common/liblog/log")

add_definitions(-DHAVE_OBSCONFIG_H -DHAVE_PLSCONFIG_H -DUNICODE -D_UNICODE)
add_compile_definitions($<$<OR:$<BOOL:$ENV{DEBUG_API}>,$<CONFIG:DEBUG>>:DEBUG_API>)

add_executable(obs-build build.cpp)

add_custom_command(TARGET obs-build
	PRE_BUILD
	COMMAND $ENV{ComSpec} /c
		$ENV{PRISM_DIR}/obs-build
		$ENV{OBS_BUILD_DIR}
		$ENV{CEF_ROOT_DIR}
		$<CONFIGURATION>)

add_executable(obs-copy build.cpp)
add_dependencies(obs-copy obs-build)

add_custom_command(TARGET obs-copy
	POST_BUILD
	COMMAND "${CMAKE_COMMAND}"
		"-DCONFIG=$<CONFIGURATION>"
		"-DOBS_DIR=$ENV{OBS_DIR}"
		"-DPRISM_DIR=$ENV{PRISM_DIR}"
		"-DBIN_DIR=$ENV{BIN_DIR}"
		"-DARCH=$ENV{ARCH}"
		-P "$ENV{PRISM_DIR}/cmake/copy_obs_bins.cmake"
	VERBATIM)

add_subdirectory(common)
add_subdirectory(main)
add_subdirectory(plugins)
if(EXISTS $ENV{PRISM_DIR}/sub/PRISMLogger/CMakeLists.txt)
	add_subdirectory(sub/PRISMLogger)
endif()

add_executable(prism-copy build.cpp)
add_dependencies(prism-copy
	PRISMLiveStudio)

add_custom_command(TARGET prism-copy
	POST_BUILD
	COMMAND "${CMAKE_COMMAND}"
		"-DCONFIG=$<CONFIGURATION>"
		"-DOBS_DIR=$ENV{OBS_DIR}"
		"-DPRISM_DIR=$ENV{PRISM_DIR}"
		"-DBIN_DIR=$ENV{BIN_DIR}"
		"-DARCH=$ENV{ARCH}"
		"-DQTDIR=$ENV{QTDIR}"
		-P "$ENV{PRISM_DIR}/cmake/copy_prism_bins.cmake"
	VERBATIM)

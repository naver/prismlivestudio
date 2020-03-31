project(frontend-tools)

include_directories("$ENV{OBS_DIR}/deps/obs-scripting")
include_directories("$ENV{PRISM_BUILD_DIR}/$ENV{MULTI_ARCH}/config")
include_directories("${CMAKE_CURRENT_BINARY_DIR}")
link_directories("$ENV{OBS_BUILD_DIR}/$ENV{MULTI_ARCH}/deps/obs-scripting")
add_definitions(-D_PRISM_PLUGIN)

configure_file(
	"${CMAKE_CURRENT_SOURCE_DIR}/frontend-tools-config.h.in"
	"${CMAKE_BINARY_DIR}/config/frontend-tools-config.h")

set(frontend-tools_HEADERS
	${frontend-tools_HEADERS}
	"${CMAKE_BINARY_DIR}/config/frontend-tools-config.h"
	auto-scene-switcher.hpp
	output-timer.hpp
	tool-helpers.hpp
	../../PLSLabel.hpp
	../../PLSRegionCapture.h
	../../PLSPushButton.h
	../../PLSCommonScrollBar.h
	../../properties-view.hpp
	../../properties-view.moc.hpp
	../../horizontal-scroll-area.hpp
	../../vertical-scroll-area.hpp
	../../double-slider.hpp
	../../slider-ignorewheel.hpp
	../../spinbox-ignorewheel.hpp
	../../spinbox.hpp
	)
set(frontend-tools_SOURCES
	${frontend-tools_SOURCES}
	auto-scene-switcher.cpp
	frontend-tools.c
	output-timer.cpp
	../../PLSLabel.cpp
	../../PLSRegionCapture.cpp
	../../PLSPushButton.cpp
	../../PLSCommonScrollBar.cpp
	../../properties-view.cpp
	../../horizontal-scroll-area.cpp
	../../vertical-scroll-area.cpp
	../../double-slider.cpp
	../../slider-ignorewheel.cpp
	../../spinbox-ignorewheel.cpp
	../../spinbox.cpp)
set(frontend-tools_UI
	${frontend-tools_UI}
	forms/auto-scene-switcher.ui
	forms/output-timer.ui)

if(SCRIPTING_ENABLED)
	set(frontend-tools_HEADERS
		${frontend-tools_HEADERS}
		scripts.hpp
		)
	set(frontend-tools_SOURCES
		${frontend-tools_SOURCES}
		scripts.cpp
		)
	set(frontend-tools_UI
		${frontend-tools_UI}
		forms/scripts.ui
		)
	set(EXTRA_LIBS
		${EXTRA_LIBS}
		obs-scripting
		)
endif()

set(frontend-tools_PLATFORM_SOURCES
	auto-scene-switcher-win.cpp)

if(BUILD_CAPTIONS)
	set(frontend-tools_PLATFORM_SOURCES
		${frontend-tools_PLATFORM_SOURCES}
		captions.cpp
		captions-handler.cpp
		captions-mssapi.cpp
		captions-mssapi-stream.cpp)
	set(frontend-tools_PLATFORM_HEADERS
		captions.hpp
		captions-handler.hpp
		captions-mssapi.hpp
		captions-mssapi-stream.hpp)
	set(frontend-tools_PLATFORM_UI
		forms/captions.ui)
endif()

qt5_wrap_ui(frontend-tools_UI_HEADERS
	${frontend-tools_UI}
	${frontend-tools_PLATFORM_UI})

add_library(frontend-tools MODULE
	${frontend-tools_HEADERS}
	${frontend-tools_SOURCES}
	${frontend-tools_UI_HEADERS}
	${frontend-tools_PLATFORM_SOURCES}
	${frontend-tools_PLATFORM_HEADERS})
target_link_libraries(frontend-tools
	${frontend-tools_PLATFORM_LIBS}
	${EXTRA_LIBS}
	obs-frontend-api
	frontend-api
	Qt5::Widgets
	liblog
	log)
add_obs_dependency(frontend-tools)
link_obs(frontend-tools)

install_obs_plugin_with_data(frontend-tools data)
copy_prism_plugin_target(frontend-tools)

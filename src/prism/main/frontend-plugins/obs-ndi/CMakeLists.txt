project(obs-ndi)

message(STATUS "OBS Plugin: ${PROJECT_NAME}")

add_definitions(-D_PRISM_PLUGIN)

include_directories("lib/ndi")

set(obs-ndi_SOURCES
	src/obs-ndi.cpp
	src/obs-ndi-source.cpp
	src/obs-ndi-output.cpp
	src/obs-ndi-filter.cpp
	src/premultiplied-alpha-filter.cpp
	src/main-output.cpp
	src/preview-output.cpp
	src/Config.cpp
	src/forms/output-settings.cpp)

set(obs-ndi_HEADERS
	src/obs-ndi.h
	src/main-output.h
	src/preview-output.h
	src/Config.h
	src/forms/output-settings.h)

set(obs-ndi_UI
	src/forms/output-settings.ui)

qt5_wrap_ui(obs-ndi_UI_HEADERS
	${obs-ndi_UI})

add_library(obs-ndi MODULE
	${obs-ndi_SOURCES}
	${obs-ndi_HEADERS}
	${obs-ndi_UI_HEADERS})
target_link_libraries(obs-ndi
	obs-frontend-api
	frontend-api
	w32-pthreads
	Qt5::Widgets)
add_obs_dependency(obs-ndi)
link_obs(obs-ndi)

copy_prism_plugin_target(obs-ndi)

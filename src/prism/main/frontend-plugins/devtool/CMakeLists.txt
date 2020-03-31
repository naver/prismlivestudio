project(devtool)

message(STATUS "PRISM Plugin: ${PROJECT_NAME}")

find_package(Qt5Network)

include_directories(${Qt5Widgets_PRIVATE_INCLUDE_DIRS} ${Qt5Network_INCLUDE_DIRS})
add_definitions(-D_PRISM_PLUGIN)

set(devtool_SOURCES
	devtool.cpp
	stylesheet.cpp)

set(devtool_HEADERS
	devtool.h)

set(devtool_UI
	devtool.ui)

qt5_wrap_ui(devtool_UI_HEADERS
	${devtool_UI})

add_library(devtool MODULE
	${devtool_SOURCES}
	${devtool_HEADERS}
	${devtool_UI_HEADERS})
target_link_libraries(devtool
	frontend-api
	Qt5::Widgets)
add_obs_dependency(devtool)
link_obs(devtool)

copy_prism_plugin_target(devtool)

project(frontend-api)

find_package(Qt5Widgets)
find_package(Qt5Widgets REQUIRED)

add_definitions(-DFRONTEND_API_LIB)

set(frontend-api_SOURCES
	frontend-api.cpp
	login-info.cpp
	dialog-view.cpp
	wizard-view.cpp
	alert-view.cpp
	dialogbuttonbox.cpp
	toast-view.cpp
	color-dialog-view.cpp
	font-dialog-view.cpp
	combobox.cpp
	pls-net-url.cpp
	qcolordialog.cpp
	PLSWidgetDpiAdapter.cpp
	../themes/PLSThemeManager.cpp
	../themes/PLSDpiHelper.cpp)

set(frontend-api_HEADERS
	frontend-internal.hpp
	frontend-api.h
	login-info.hpp
	dialog-view.hpp
	wizard-view.hpp
	alert-view.hpp
	dialogbuttonbox.hpp
	toast-view.hpp
	color-dialog-view.hpp
	font-dialog-view.hpp
	combobox.hpp
	cancel.hpp
	qcolordialog.hpp
	PLSToplevelView.hpp
	PLSWidgetDpiAdapter.hpp
	../themes/PLSThemeManager.h
	../themes/PLSDpiHelper.h)

set(frontend-api_UI
	forms/PLSDialogView.ui
	forms/PLSAlertView.ui
	forms/PLSToastView.ui)

qt5_wrap_ui(frontend-api_UI_HEADERS ${frontend-api_UI})

add_library(frontend-api SHARED
	${frontend-api_SOURCES}
	${frontend-api_HEADERS}
	${frontend-api_UI_HEADERS})
add_dependencies(frontend-api
	obs-build
	obs-copy)
include_directories($ENV{PRISM_DIR}/libs/HMAC/include)
if (MSVC)
	target_link_libraries(frontend-api
		obs
		obs-frontend-api
		liblog
		Qt5::Widgets
		Qt5::Network)
	link_hmac(frontend-api)
else()
	target_link_libraries(frontend-api
		libobs
		obs-frontend-api
		Qt5::Widgets)
endif()

if(UNIX AND NOT APPLE)
	set_target_properties(frontend-api
		PROPERTIES
			OUTPUT_NAME frontend-api
			VERSION 0.0
			SOVERSION 0
			)
endif()

install_obs_core(frontend-api)

copy_prism_target(frontend-api)

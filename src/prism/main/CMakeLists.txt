if (BROWSER_AVAILABLE_INTERNAL)
add_definitions(-DBROWSER_AVAILABLE)
endif()

#-- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

project(PRISMLiveStudio)

set(DISABLE_UPDATE_MODULE TRUE CACHE BOOL "Disables building the update module")

if(NOT DEFINED TWITCH_CLIENTID OR "${TWITCH_CLIENTID}" STREQUAL "" OR
   NOT DEFINED TWITCH_HASH     OR "${TWITCH_HASH}"     STREQUAL "" OR
   NOT BROWSER_AVAILABLE_INTERNAL)
	set(TWITCH_ENABLED FALSE)
	set(TWITCH_CLIENTID "")
	set(TWITCH_HASH "0")
else()
	set(TWITCH_ENABLED TRUE)
endif()

if(NOT DEFINED MIXER_CLIENTID OR "${MIXER_CLIENTID}" STREQUAL "" OR
   NOT DEFINED MIXER_HASH     OR "${MIXER_HASH}"     STREQUAL "" OR
   NOT BROWSER_AVAILABLE_INTERNAL)
	set(MIXER_ENABLED FALSE)
	set(MIXER_CLIENTID "")
	set(MIXER_HASH "0")
else()
	set(MIXER_ENABLED TRUE)
endif()

if(NOT DEFINED RESTREAM_CLIENTID OR "${RESTREAM_CLIENTID}" STREQUAL "" OR
   NOT DEFINED RESTREAM_HASH     OR "${RESTREAM_HASH}"     STREQUAL "" OR
   NOT BROWSER_AVAILABLE_INTERNAL)
	set(RESTREAM_ENABLED FALSE)
	set(RESTREAM_CLIENTID "")
	set(RESTREAM_HASH "0")
else()
	set(RESTREAM_ENABLED TRUE)
endif()

configure_file(ui-config.h.in "${CMAKE_CURRENT_BINARY_DIR}/ui-config.h")

configure_file(pls.rc.in "${CMAKE_CURRENT_BINARY_DIR}/pls.rc")

set(CMAKE_INCLUDE_CURRENT_DIR TRUE)
set(CMAKE_AUTOMOC TRUE)

find_package(Qt5Svg REQUIRED)
find_package(Qt5Network REQUIRED)

find_package(FFmpeg REQUIRED COMPONENTS avcodec avutil avformat)
find_package(Qt5 COMPONENTS Multimediawidgets Multimedia REQUIRED)

if(APPLE)
	find_package(Qt5MacExtras REQUIRED)
endif(APPLE)

if(NOT Qt5Widgets_FOUND)
	if (ENABLE_UI)
		message(FATAL_ERROR "Failed to find Qt5")
	else()
		message(STATUS "Qt5 not found - UI disabled")
		return()
	endif()
endif()

include_directories(${FFMPEG_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${Qt5Widgets_PRIVATE_INCLUDE_DIRS})
include_directories(SYSTEM "frontend-api")
include_directories(SYSTEM "$ENV{OBS_DIR}/libobs")
include_directories(SYSTEM "$ENV{OBS_DIR}/deps/libff")
include_directories(SYSTEM "$ENV{OBS_DIR}/deps/json11")
#channels config
include_directories(
	.
	"$ENV{PRISM_DIR}/main"
	"$ENV{PRISM_DIR}/main/channels/ChannelsDataApi"
	"$ENV{PRISM_DIR}/main/channels/ShareView"
	"$ENV{PRISM_DIR}/main/channels/LiveEnd-Summary"
	"$ENV{PRISM_DIR}/main/channels/Chat"
	"$ENV{PRISM_DIR}/main/channels/PlatformApi"
	"$ENV{PRISM_DIR}/main/channels/ChannelsSetting"
	"$ENV{PRISM_DIR}/main/PLSPlatformApi"
	"$ENV{PRISM_DIR}/main/PLSPlatformApi/common"
	"$ENV{PRISM_DIR}/main/themes"
	"$ENV{PRISM_DIR}/main/PLSPlatformApi/afreecatv"
	"$ENV{PRISM_DIR}/main/PLSPlatformApi/facebook"
	"${CMAKE_CURRENT_BINARY_DIR}"
	)
if(BROWSER_AVAILABLE_INTERNAL)
	include_directories(SYSTEM "$ENV{OBS_DIR}/plugins/obs-browser/panel")
endif()

find_package(Libcurl REQUIRED)
include_directories(${LIBCURL_INCLUDE_DIRS})
add_definitions(${LIBCURL_DEFINITIONS})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)
set(CMAKE_CXX_EXTENSIONS NO)

if(${CMAKE_C_COMPILER_ID} MATCHES "Clang" OR ${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
	set(CMAKE_COMPILER_IS_CLANG TRUE)
endif()

if(CMAKE_SYSTEM_PROCESSOR MATCHES "^(powerpc|ppc)64le")
	add_compile_definitions(NO_WARN_X86_INTRINSICS)
endif()

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX OR CMAKE_COMPILER_IS_CLANG)
	set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wvla -Wno-unused-function -Wno-missing-field-initializers ${CMAKE_CXX_FLAGS} -fno-strict-aliasing")
	set(CMAKE_C_FLAGS "-Wall -Wextra -Wvla -Wno-unused-function -Werror-implicit-function-declaration -Wno-missing-braces -Wno-missing-field-initializers ${CMAKE_C_FLAGS} -std=gnu99 -fno-strict-aliasing")

	option(USE_LIBC++ "Use libc++ instead of libstdc++" ${APPLE})
	if(USE_LIBC++)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
	endif()
elseif(MSVC)
	if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
		string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
	else()
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
	endif()

#Disable pointless constant condition warnings
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4127 /wd4201 /wd4456 /wd4457 /wd4458 /wd4459 /wd4595")
endif()

if(WIN32)
	add_definitions(-DUNICODE -D_UNICODE -D_CRT_SECURE_NO_WARNINGS -D_CRT_NONSTDC_NO_WARNINGS)
endif()

add_definitions(-DDL_OPENGL="libobs-opengl.dll" -DDL_D3D11="libobs-d3d11.dll")

if(MSVC)
	add_compile_options("/MP")
	set(CMAKE_C_FLAGS_DEBUG "/DDEBUG=1 /D_DEBUG=1 ${CMAKE_C_FLAGS_DEBUG}")
	set(CMAKE_CXX_FLAGS_DEBUG "/DDEBUG=1 /D_DEBUG=1 ${CMAKE_C_FLAGS_DEBUG}")

	if(NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
		set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SAFESEH:NO")
		set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} /SAFESEH:NO")
		set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} /SAFESEH:NO")
	endif()
else()
	if(MINGW)
		set(CMAKE_WIDL "widl" CACHE STRING "wine IDL header file generation program")
		set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -D_WIN32_WINNT=0x0600 -DWINVER=0x0600")
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D_WIN32_WINNT=0x0600 -DWINVER=0x0600")
	endif()
	set(CMAKE_C_FLAGS_DEBUG "-DDEBUG=1 -D_DEBUG=1 ${CMAKE_C_FLAGS_DEBUG}")
	set(CMAKE_CXX_FLAGS_DEBUG "-DDEBUG=1 -D_DEBUG=1 ${CMAKE_C_FLAGS_DEBUG}")
endif()

if(APPLE)
	set(CMAKE_MACOSX_RPATH TRUE)
	set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
	list(APPEND CMAKE_INSTALL_RPATH "@loader_path/" "@executable_path/")
elseif(UNIX)
	option(USE_XDG "Utilize XDG Base Directory Specification" ON)
	if(USE_XDG)
		add_definitions(-DUSE_XDG)
	endif()

	if(NOT UNIX_STRUCTURE)
		list(APPEND CMAKE_INSTALL_RPATH "$ORIGIN")
	endif()
endif()

add_subdirectory(frontend-api)

if (UNIX AND UNIX_STRUCTURE AND NOT APPLE)
#add_subdirectory(xdg - data)
endif()

add_subdirectory(frontend-plugins)
add_subdirectory(channels)
add_subdirectory(PLSPlatformApi)
add_subdirectory(themes)
if(WIN32)
#add_subdirectory(win - update / updater)
endif()

if(WIN32)
	include_directories(
		$ENV{OBS_BUILD_DIR}/$ENV{MULTI_ARCH}/deps/jansson/include
		$ENV{OBS_DIR}/deps/blake2/src
		${CMAKE_CURRENT_SOURCE_DIR}/deps/nelo/include
		$ENV{PRISM_DIR}/libs/HMAC/include
		$ENV{PRISM_DIR}/plugins/prism-channel-controller/prism-channel-controller-plugin)
	link_directories(
		$ENV{OBS_BUILD_DIR}/$ENV{MULTI_ARCH}/libobs
		$ENV{OBS_BUILD_DIR}/$ENV{MULTI_ARCH}/deps/jansson/lib
		$ENV{OBS_BUILD_DIR}/$ENV{MULTI_ARCH}/deps/blake2
		$ENV{OBS_BUILD_DIR}/$ENV{MULTI_ARCH}/deps/w32-pthreads)

	set(pls_PLATFORM_SOURCES
		platform-windows.cpp
		"${CMAKE_CURRENT_BINARY_DIR}/pls.rc")
	set(pls_PLATFORM_HEADERS)
	set(pls_PLATFORM_UIS)
	set(pls_PLATFORM_LIBRARIES
		crypt32
		blake2)

	if(CMAKE_SIZEOF_VOID_P EQUAL 4)
		set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /LARGEADDRESSAWARE")
	endif()
elseif(APPLE)
	set(pls_PLATFORM_SOURCES
		platform-osx.mm)

	find_package(AppKit REQUIRED)
	set(pls_PLATFORM_LIBRARIES ${APPKIT_LIBRARIES})

	add_definitions(-fobjc-arc)

	option(ENABLE_SPARKLE_UPDATER "Enables updates via the Sparkle framework (don't forget to update the Info.plist for your .app)" OFF)
	if(ENABLE_SPARKLE_UPDATER)
		find_library(SPARKLE Sparkle)
		include_directories(${SPARKLE})
		set(pls_PLATFORM_SOURCES
			${pls_PLATFORM_SOURCES}
			sparkle-updater.mm)
		set(pls_PLATFORM_LIBRARIES
			${pls_PLATFORM_LIBRARIES}
			${SPARKLE})
		add_definitions(-DUPDATE_SPARKLE=1)
	endif()

elseif(UNIX)
        find_package(Qt5X11Extras REQUIRED)

	set(pls_PLATFORM_SOURCES
		platform-x11.cpp)

        set(pls_PLATFORM_LIBRARIES
                Qt5::X11Extras)
endif()

if(BROWSER_AVAILABLE_INTERNAL)
	list(APPEND pls_PLATFORM_SOURCES
		obf.c
		auth-oauth.cpp
		window-dock-browser.cpp
		window-extra-browsers.cpp
		)
	list(APPEND pls_PLATFORM_HEADERS
		obf.h
		auth-oauth.hpp
		window-dock-browser.hpp
		window-extra-browsers.hpp
		)

	if(TWITCH_ENABLED)
		list(APPEND pls_PLATFORM_SOURCES
			auth-twitch.cpp
			)
		list(APPEND pls_PLATFORM_HEADERS
			auth-twitch.hpp
			)
	endif()

	if(MIXER_ENABLED)
		list(APPEND pls_PLATFORM_SOURCES
			auth-mixer.cpp
			)
		list(APPEND pls_PLATFORM_HEADERS
			auth-mixer.hpp
			)
	endif()

	if(RESTREAM_ENABLED)
		list(APPEND pls_PLATFORM_SOURCES
			auth-restream.cpp
			)
		list(APPEND pls_PLATFORM_HEADERS
			auth-restream.hpp
			)
	endif()
endif()

set(pls_libffutil_SOURCES
	../../obs/deps/libff/libff/ff-util.c
	)
set(pls_libffutil_HEADERS
	../../obs/deps/libff/libff/ff-util.h
	)

if(MSVC)
	set_source_files_properties(
		../../obs/deps/libff/libff/ff-util.c
		PROPERTIES COMPILE_FLAGS -Dinline=__inline
		)
	set(pls_PLATFORM_LIBRARIES
		${pls_PLATFORM_LIBRARIES}
		w32-pthreads)
endif()

set(pls_SOURCES
	${pls_PLATFORM_SOURCES}
	${pls_libffutil_SOURCES}
	../../obs/deps/json11/json11.cpp
	pls-app.cpp
	window-dock.cpp
	api-interface.cpp
	window-basic-main.cpp
	window-basic-stats.cpp
	window-basic-filters.cpp
	window-basic-settings.cpp
	window-basic-properties.cpp
	window-basic-auto-config.cpp
	window-basic-main-outputs.cpp
	window-basic-source-select.cpp
	window-basic-settings-stream.cpp
	window-basic-auto-config-test.cpp
	window-basic-main-scene-collections.cpp
	window-basic-main-transitions.cpp
	window-basic-main-dropfiles.cpp
	window-basic-main-profiles.cpp
	window-basic-main-browser.cpp
	window-basic-main-icons.cpp
	window-basic-status-bar.cpp
	window-basic-adv-audio.cpp
	window-basic-transform.cpp
	window-basic-preview.cpp
	window-basic-about.cpp
	window-namedialog.cpp
	window-log-reply.cpp
	window-projector.cpp
	window-remux.cpp
	auth-base.cpp
	source-tree.cpp
	properties-view.cpp
	focus-list.cpp
	menu-button.cpp
	double-slider.cpp
	slider-ignorewheel.cpp
	spinbox-ignorewheel.cpp
	record-button.cpp
	url-push-button.cpp
	volume-control.cpp
	adv-audio-control.cpp
	item-widget-helpers.cpp
	horizontal-scroll-area.cpp
	vertical-scroll-area.cpp
	visibility-item-widget.cpp
	slider-absoluteset-style.cpp
	PLSAddSourceMenuStyle.cpp
	PLSAction.cpp
	qt-display.cpp
	crash-report.cpp
	hotkey-edit.cpp
	source-label.cpp
	remote-text.cpp
	audio-encoders.cpp
	qt-wrappers.cpp
	main-view.cpp
	spinbox.cpp
	browser-view.cpp
	network-access-manager.cpp
	network-environment.cpp
	loading-event.cpp
	json-data-handler.cpp
	login-common-helper.cpp
	pls-gpop-data.cpp
	pls-complex-header-icon.cpp
	color-circle.cpp
	about-view.cpp
	custom-help-menu-item.cpp
	PLSFileButton.cpp
	PLSFileItemView.cpp
	PLSOpenSourceView.cpp
	PLSSceneDataMgr.cpp
	PLSSceneItemView.cpp
	PLSSceneListView.cpp
	PLSScrollAreaContent.cpp
	PLSSceneTransitionsView.cpp
	PLSFiltersItemView.cpp
	PLSFiltersListView.cpp
	PLSColorFilterView.cpp
	color-circle.cpp
	PLSHttpApi/PLSHttpHelper.cpp
	PLSHttpApi/PLSNetworkReplyBuilder.cpp
	PLSHttpApi/PLSHmacNetworkReplyBuilder.cpp
	PLSMenu.cpp
	PLSLivingMsgView.cpp
	PLSLivingMsgItem.cpp
	PLSToastButton.cpp
	PLSToastMsgPopup.cpp
	PLSPreviewTitle.cpp
	PLSSourceTypeList.cpp
	PLSFilterTypeList.cpp
	PLSCompleter.cpp
	PLSCommonScrollBar.cpp
	unzip.cpp
	PLSBeautyFilterView.cpp
	PLSBeautySlider.cpp
	PLSBeautyFaceItemView.cpp
	PLSBeautyDataMgr.cpp
	layout/flowlayout.cpp
	PLSBackgroundMusicView.cpp
	window-basic-main-bgm.cpp
	PLSBgmDataManager.cpp
	PLSBgmItemView.cpp
	PLSScrollingLabel.cpp
	PLSBgmDragView.cpp
	PLSBgmItemCoverView.cpp
	PLSLabel.cpp
	media-slider.cpp
	PLSMediaController.cpp
	PLSGipyStickerView.cpp
	GiphyDownloader.cpp
	GiphyWebHandler.cpp
	PLSSearchPopupMenu.cpp
	PLSPushButton.cpp
	PLSFloatScrollBarScrollArea.cpp
	PLSNetworkMonitor.cpp
	TextMotionTemplateButton.cpp
	TextMotionTemplateDataHelper.cpp
	PLSBgmLibraryView.cpp
	PLSToastMsgFrame.cpp
	PLSBlockDump.cpp
	PLSRegionCapture.cpp
	PLSBgmItemDelegate.cpp
	)

set(pls_HEADERS
	${pls_PLATFORM_HEADERS}
	${pls_libffutil_HEADERS}
	../../obs/deps/json11/json11.hpp
	pls-app.hpp
	platform.hpp
	window-dock.hpp
	window-main.hpp
	window-basic-main.hpp
	window-basic-stats.hpp
	window-basic-filters.hpp
	window-basic-settings.hpp
	window-basic-properties.hpp
	window-basic-auto-config.hpp
	window-basic-main-outputs.hpp
	window-basic-source-select.hpp
	window-basic-about.hpp
	window-basic-status-bar.hpp
	window-basic-adv-audio.hpp
	window-basic-transform.hpp
	window-basic-preview.hpp
	window-namedialog.hpp
	window-log-reply.hpp
	window-projector.hpp
	window-remux.hpp
	auth-base.hpp
	source-tree.hpp
	properties-view.hpp
	properties-view.moc.hpp
	display-helpers.hpp
	balance-slider.hpp
	double-slider.hpp
	slider-ignorewheel.hpp
	spinbox-ignorewheel.hpp
	focus-list.hpp
	menu-button.hpp
	mute-checkbox.hpp
	record-button.hpp
	url-push-button.hpp
	volume-control.hpp
	adv-audio-control.hpp
	item-widget-helpers.hpp
	visibility-checkbox.hpp
	locked-checkbox.hpp
	horizontal-scroll-area.hpp
	expand-checkbox.hpp
	vertical-scroll-area.hpp
	visibility-item-widget.hpp
	slider-absoluteset-style.hpp
	PLSAddSourceMenuStyle.hpp
	PLSAction.h
	qt-display.hpp
	crash-report.hpp
	hotkey-edit.hpp
	source-label.hpp
	remote-text.hpp
	audio-encoders.hpp
	qt-wrappers.hpp
	clickable-label.hpp
	main-view.hpp
	spinbox.hpp
	browser-view.hpp
	pls-common-define.hpp
	pls-net-url.hpp
	free-network-reply-guard.hpp
	json-data-handler.hpp
	login-common-helper.hpp
	network-access-manager.hpp
	pls-common-language.hpp
	network-environment.hpp
	loading-event.hpp
	pls-gpop-data.hpp
	pls-gpop-data-struct.hpp
	pls-complex-header-icon.hpp
	color-circle.hpp
	about-view.hpp
	custom-help-menu-item.hpp
	PLSFileButton.hpp
	PLSFileItemView.hpp
	PLSOpenSourceView.h
	PLSSceneDataMgr.h
	PLSSceneItemView.h
	PLSSceneListView.h
	PLSScrollAreaContent.h
	PLSSceneTransitionsView.h
	PLSFiltersItemView.h
	PLSFiltersListView.h
	PLSColorFilterView.h
	color-circle.hpp
	PLSHttpApi/PLSHttpHelper.h
	PLSHttpApi/PLSNetworkReplyBuilder.h
	PLSHttpApi/PLSHmacNetworkReplyBuilder.h
	PLSMenu.hpp
	PLSLivingMsgItem.hpp
	PLSLivingMsgView.hpp
	PLSToastButton.hpp
	PLSToastMsgPopup.hpp
	PLSPreviewTitle.h
	PLSCompleter.hpp
	PLSCommonScrollBar.h
	unzip.h
	PLSBeautyFilterView.h
	PLSBeautySlider.h
	PLSBeautyFaceItemView.h
	PLSBeautyDataMgr.h
	PLSBeautyDefine.h
	layout/flowlayout.h
	PLSBackgroundMusicView.h
	PLSBgmDataManager.h
	PLSBgmItemView.h
	PLSScrollingLabel.h
	PLSBgmDragView.h
	PLSBgmItemCoverView.h
	PLSLabel.hpp
	media-slider.h
	PLSMediaController.h
	PLSGipyStickerView.h
	GiphyDefine.h
	GiphyDownloader.h
	GiphyWebHandler.h
	PLSSearchPopupMenu.h
	PLSPushButton.h
	PLSFloatScrollBarScrollArea.h
	PLSNetworkMonitor.h
	TextMotionTemplateButton.h
	TextMotionTemplateDataHelper.h
	PLSBgmLibraryView.h
	PLSToastMsgFrame.h
	PLSBlockDump.h
	PLSRegionCapture.h
	PLSBgmItemDelegate.h
	)


set(pls_UI
	forms/NameDialog.ui
	forms/AutoConfigStartPage.ui
	forms/AutoConfigVideoPage.ui
	forms/AutoConfigStreamPage.ui
	forms/AutoConfigTestPage.ui
	forms/ColorSelect.ui
	forms/PLSLogReply.ui
	forms/PLSBasic.ui
	forms/PLSBasicTransform.ui
	forms/PLSBasicFilters.ui
	forms/PLSBasicSettings.ui
	forms/PLSBasicSourceSelect.ui
	forms/PLSExtraBrowsers.ui
	forms/PLSRemux.ui
	forms/PLSAbout.ui
	forms/PLSMainView.ui
	forms/PLSBrowserView.ui
	forms/PLSComplexHeaderIcon.ui
	forms/PLSAboutView.ui
	forms/PLSCustomHelpMenuItem.ui
	forms/PLSFileButton.ui
	forms/PLSFileItemView.ui
	forms/PLSOpenSourceView.ui
	${pls_PLATFORM_UIS}
	forms/PLSSceneItemView.ui
	forms/PLSSceneListView.ui
	forms/PLSSceneTransitionsView.ui
	forms/PLSFiltersItemView.ui
	forms/PLSFiltersListView.ui
	forms/PLSColorFilterView.ui
	${channels-UI_FILES}
	${PLSPlatformApi-UI_FILES}
	forms/PLSBasicStats.ui
	forms/PLSLivingMsgItem.ui
	forms/PLSLivingMsgView.ui
	forms/PLSToastButton.ui
	forms/PLSToastMsgPopup.ui
	forms/PLSBeautyFilterView.ui
	forms/PLSBeautyFaceItemView.ui
	forms/PLSBackgroundMusicView.ui
	forms/PLSBgmItemView.ui
	forms/PLSBgmItemCoverView.ui
	forms/PLSMediaController.ui
	forms/PLSBeautyFaceItemView.ui
	forms/PLSGipyStickerView.ui
	forms/PLSSearchPopupMenu.ui
	forms/TextMotionTemplateButton.ui
        forms/PLSBgmLibraryView.ui
        forms/PLSBgmLibraryItem.ui
	)

set(pls_QRC
	forms/PLSPrismLogin.qrc
	forms/pls.qrc
	${channels-RCC_FILES})

qt5_wrap_ui(pls_UI_HEADERS ${pls_UI})
qt5_add_resources(pls_QRC_SOURCES ${pls_QRC})

file(GLOB pls_LOG_SOURCES log/*.c log/*.cpp log/*.cxx log/*.cc)
file(GLOB pls_LOG_HEADERS log/*.h log/*.hpp log/*.hxx log/*.hh)
add_executable(PRISMLiveStudio WIN32
	${pls_SOURCES}
	${pls_HEADERS}
	${pls_UI_HEADERS}
	${pls_QRC_SOURCES}
	${pls_LOG_SOURCES}
	${pls_LOG_HEADERS}
	${channels-SRC_FILES}
	${channels-HDR_FILES}
	${PLSPlatformApi-SRC_FILES}
	${PLSPlatformApi-HDR_FILES})
add_obs_dependency(PRISMLiveStudio)
add_dependencies(PRISMLiveStudio
    frontend-api)

if (MSVC)
	target_link_libraries(PRISMLiveStudio
		Qt5::Widgets
		Qt5::Network
		Qt5::Svg
		Qt5::Multimedia
		frontend-api
		liblog
		Version
		${FFMPEG_LIBRARIES}
		${LIBCURL_LIBRARIES}
		${pls_PLATFORM_LIBRARIES}
		)
    link_obs(PRISMLiveStudio)
    link_jansson(PRISMLiveStudio)
    link_hmac(PRISMLiveStudio)
else()
	target_link_libraries(PRISMLiveStudio
		libobs
		Qt5::Widgets
		Qt5::Network
		Qt5::Svg
		frontend-api
		liblog
		${FFMPEG_LIBRARIES}
		${LIBCURL_LIBRARIES}
		${pls_PLATFORM_LIBRARIES})
endif()

	
if(MSVC)
  file(GENERATE
    OUTPUT "$ENV{PRISM_BUILD_DIR}/$ENV{MULTI_ARCH}/ALL_BUILD.vcxproj.user"
    INPUT "$ENV{PRISM_DIR}/cmake/ALL_BUILD.vcxproj.user.in")
endif()

if (APPLE)
	target_link_libraries(PRISMLiveStudio
			Qt5::MacExtras)
	set_target_properties(PRISMLiveStudio PROPERTIES LINK_FLAGS "-pagezero_size 10000 -image_base 100000000")
endif()


#channels config end

define_graphic_modules(PRISMLiveStudio)

install_obs_core(PRISMLiveStudio)
install_obs_data(PRISMLiveStudio data prism-studio)
install_obs_data_file(PRISMLiveStudio ../AUTHORS prism-studio/authors)

copy_prism_target(PRISMLiveStudio)

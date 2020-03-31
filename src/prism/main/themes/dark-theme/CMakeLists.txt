project(dark-theme)

message(STATUS "PRISM Theme Plugin: dark-theme")

find_package(Qt5Widgets)
find_package(Qt5Widgets REQUIRED)

set(CMAKE_AUTOMOC True)
set(CMAKE_AUTORCC True)

file(GLOB_RECURSE css_files *.css *.qss)
source_group("css" FILES ${css_files})
set_source_files_properties(${css_files} PROPERTIES HEADER_FILE_ONLY True)

file(GLOB_RECURSE files *.c *.cpp *.cxx *.cc *.h *.hpp *.hxx *.hh *.qrc)

add_library(dark-theme SHARED ${files} ${css_files})
target_link_libraries(dark-theme Qt5::Widgets)

# install_obs_core(dark-theme)
copy_prism_theme_target(dark-theme)

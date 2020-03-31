

set(CMAKE_AUTOMOC True)
set(CMAKE_AUTOUIC True)
file(GLOB_RECURSE channels-SRC_FILES *.c *.cpp *.cxx *.cc)
file(GLOB_RECURSE channels-HDR_FILES *.h *.hpp *.hxx *.hh)
file(GLOB_RECURSE channels-UI_FILES *.ui)
file(GLOB_RECURSE channels-RCC_FILES *.qrc)

set(channels-SRC_FILES ${channels-SRC_FILES} PARENT_SCOPE)
set(channels-HDR_FILES ${channels-HDR_FILES} PARENT_SCOPE)
set(channels-UI_FILES ${channels-UI_FILES} PARENT_SCOPE)
set(channels-RCC_FILES ${channels-RCC_FILES} PARENT_SCOPE)



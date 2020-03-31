

set(CMAKE_AUTOMOC True)
set(CMAKE_AUTOUIC True)
file(GLOB_RECURSE PLSPlatformApi-SRC_FILES *.c *.cpp *.cxx *.cc)
file(GLOB_RECURSE PLSPlatformApi-HDR_FILES *.h *.hpp *.hxx *.hh)
file(GLOB_RECURSE PLSPlatformApi-UI_FILES *.ui)

set(PLSPlatformApi-SRC_FILES ${PLSPlatformApi-SRC_FILES} PARENT_SCOPE)
set(PLSPlatformApi-HDR_FILES ${PLSPlatformApi-HDR_FILES} PARENT_SCOPE)
set(PLSPlatformApi-UI_FILES ${PLSPlatformApi-UI_FILES} PARENT_SCOPE)


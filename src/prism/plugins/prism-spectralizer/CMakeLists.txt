if (NOT WIN32)
	return()
endif()

if (MSVC)
	set(prism-spectralizer_PLATFORM_DEPS
            w32-pthreads)
endif ()

message(STATUS "PRISM Plugin: ${PLUGIN_NAME}")

project(${PLUGIN_NAME})

add_definitions(-D_PRISM_PLUGIN)

math(EXPR BITS "8*${CMAKE_SIZEOF_VOID_P}")
set(FFTW_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/fftw3/include)
set(FFTW_LIBRARIES ${CMAKE_CURRENT_SOURCE_DIR}/fftw3/lib/${BITS}bit/libfftw3-3.lib)

include_directories(${FFTW_INCLUDE_DIRS})

file(GLOB_RECURSE src_files *.c *.cpp *.cxx *.cc)
file(GLOB_RECURSE hdr_files *.h *.hpp *.hxx *.hh)

add_library(${PLUGIN_NAME} MODULE ${src_files} ${hdr_files})
target_link_libraries(${PLUGIN_NAME}
	liblog
	log
	gdiplus
	d3d9
	d3d11
	dxgi
	obs-frontend-api
	${FFTW_LIBRARIES}
        ${prism-spectralizer_PLATFORM_DEPS})

add_obs_dependency(${PLUGIN_NAME})
link_obs(${PLUGIN_NAME})

install_obs_plugin_with_data(${PLUGIN_NAME} data)
copy_prism_plugin_target(${PLUGIN_NAME})

set(FFTW_BINARY libfftw3-3.dll)
math(EXPR BITS "8*${CMAKE_SIZEOF_VOID_P}")
add_custom_command(TARGET ${PLUGIN_NAME} POST_BUILD
          COMMAND ${CMAKE_COMMAND} -E copy
          "${CMAKE_CURRENT_SOURCE_DIR}/fftw3/lib/${BITS}bit/${FFTW_BINARY}"
          "${CMAKE_BINARY_DIR}/rundir/$<CONFIGURATION>/obs-plugins/${BITS}bit/"
)

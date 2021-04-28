project(${PLUGIN_NAME})

find_package(FFmpeg REQUIRED
	COMPONENTS avcodec avutil avformat)

add_definitions(-D_PRISM_PLUGIN)

math(EXPR BITS "8*${CMAKE_SIZEOF_VOID_P}")

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/x265/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../../../obs/dependencies/win${BITS}/include)

file(GLOB_RECURSE src_files *.c *.cpp *.cxx *.cc)
file(GLOB_RECURSE hdr_files *.h *.hpp *.hxx *.hh)

add_library(${PLUGIN_NAME} MODULE ${src_files} ${hdr_files})
target_link_libraries(${PLUGIN_NAME}
	liblog
	log
	obs
	${CMAKE_CURRENT_SOURCE_DIR}/x265/lib/${BITS}bit/x265-static.lib
	${CMAKE_CURRENT_SOURCE_DIR}/../../../obs/dependencies/win${BITS}/bin/avcodec.lib
	${CMAKE_CURRENT_SOURCE_DIR}/../../../obs/dependencies/win${BITS}/bin/avformat.lib
	${CMAKE_CURRENT_SOURCE_DIR}/../../../obs/dependencies/win${BITS}/bin/avutil.lib)

add_obs_dependency(${PLUGIN_NAME})
link_obs(${PLUGIN_NAME})

install_obs_plugin_with_data(${PLUGIN_NAME} data)
copy_prism_plugin_target(${PLUGIN_NAME})
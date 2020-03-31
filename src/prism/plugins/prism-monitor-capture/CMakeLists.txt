if (NOT WIN32)
	return()
endif()

message(STATUS "PRISM Plugin: ${PLUGIN_NAME}")

project(${PLUGIN_NAME})

file(GLOB_RECURSE src_files *.c *.cpp *.cxx *.cc)
file(GLOB_RECURSE hdr_files *.h *.hpp *.hxx *.hh)

add_definitions(-D_PRISM_PLUGIN)

add_library(${PLUGIN_NAME} MODULE ${src_files} ${hdr_files})
target_link_libraries(${PLUGIN_NAME}
	liblog
	log
	gdiplus
	d3d9
	d3d11
	dxgi)
add_obs_dependency(${PLUGIN_NAME})
link_obs(${PLUGIN_NAME})

install_obs_plugin_with_data(${PLUGIN_NAME} data)
copy_prism_plugin_target(${PLUGIN_NAME})


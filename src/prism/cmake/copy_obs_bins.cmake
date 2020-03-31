# CONFIG OBS_DIR PRISM_DIR BIN_DIR ARCH

if(${CONFIG} STREQUAL "RelWithDebInfo")
	set(CONFIG_SRC Release)
else()
	set(CONFIG_SRC ${CONFIG})
endif()

set(OUTPUT_DIR ${BIN_DIR}/${CONFIG_SRC}/${ARCH})

message(STATUS "Copy prism bins.")
message(STATUS "OBS_DIR=${OBS_DIR}")
message(STATUS "PRISM_DIR=${PRISM_DIR}")
message(STATUS "OUTPUT_DIR=${OUTPUT_DIR}")

if(${ARCH} STREQUAL "Win32")
	file(GLOB deps ${OBS_DIR}/dependencies/win32/bin/*.dll)
else()
	file(GLOB deps ${OBS_DIR}/dependencies/win64/bin/*.dll)
endif()
foreach(dep ${deps})
	message(STATUS "Copy file, from ${dep} to ${OUTPUT_DIR}")
	file(COPY "${dep}" DESTINATION "${OUTPUT_DIR}")
endforeach()

execute_process(COMMAND $ENV{ComSpec} /c
	${PRISM_DIR}/obs-copy-bins
	${OBS_DIR}
	${PRISM_DIR}
	${OUTPUT_DIR}
	${CONFIG})

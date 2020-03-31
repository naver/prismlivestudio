
# CONFIG OBS_DIR PRISM_DIR BIN_DIR ARCH QTDIR

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

file(GLOB nelodeps ${PRISM_DIR}/main/deps/nelo/bin/${CONFIG_SRC}/${ARCH}/*)
foreach(nelodep ${nelodeps})
	message(STATUS "Copy file, from ${nelodep} to ${OUTPUT_DIR}")
	file(COPY "${nelodep}" DESTINATION "${OUTPUT_DIR}")
endforeach()

file(GLOB openssldeps ${PRISM_DIR}/main/deps/openssl/${ARCH}/*)
foreach(openssldep ${openssldeps})
	message(STATUS "Copy file, from ${openssldep} to ${OUTPUT_DIR}")
	file(COPY "${openssldep}" DESTINATION "${OUTPUT_DIR}")
endforeach()

file(GLOB hmacs ${PRISM_DIR}/libs/HMAC/bin/${CONFIG_SRC}/${ARCH}/*)
foreach(hmac ${hmacs})
	message(STATUS "Copy file, from ${hmac} to ${OUTPUT_DIR}")
	file(COPY "${hmac}" DESTINATION "${OUTPUT_DIR}")
endforeach()

file(GLOB mosquitto_deps ${PRISM_DIR}/main/deps/mosquitto/bin/${CONFIG_SRC}/${ARCH}/*)
foreach(item ${mosquitto_deps})
	message(STATUS "Copy file, from ${item} to ${OUTPUT_DIR}")
	file(COPY "${item}" DESTINATION "${OUTPUT_DIR}")
endforeach()

execute_process(COMMAND $ENV{ComSpec} /c
	${PRISM_DIR}/prism-copy-bins
	${OBS_DIR}
	${PRISM_DIR}
	${OUTPUT_DIR}
	${CONFIG})

execute_process(
	COMMAND ${QTDIR}/bin/windeployqt ${OUTPUT_DIR}/PRISMLiveStudio.exe --plugindir plugins
	WORKING_DIRECTORY ${OUTPUT_DIR})

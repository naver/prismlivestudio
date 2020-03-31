# CONFIG PRISM_DIR SRC_DIR BIN_DIR ARCH TARGET

if(${CONFIG} STREQUAL "RelWithDebInfo")
	set(CONFIG_SRC Release)
else()
	set(CONFIG_SRC ${CONFIG})
endif()

set(OUTPUT_DIR ${BIN_DIR}/${CONFIG_SRC}/${ARCH})
set(DATA_OUTPUT_DIR ${BIN_DIR}/${CONFIG_SRC}/${ARCH}/data/prism-studio)

message(STATUS "Copy prism target.")
message(STATUS "PRISM_DIR=${PRISM_DIR}")
message(STATUS "OUTPUT_DIR=${OUTPUT_DIR}")
message(STATUS "CMAKE_CURRENT_BINARY_DIR=${CMAKE_CURRENT_BINARY_DIR}")
message(STATUS "TARGET=${TARGET}")

if(EXISTS "${CMAKE_CURRENT_BINARY_DIR}/${CONFIG}/${TARGET}.exe")
    message(STATUS "${CMAKE_CURRENT_BINARY_DIR}/${CONFIG}/${TARGET}.exe" -> "${OUTPUT_DIR}")
    file(COPY "${CMAKE_CURRENT_BINARY_DIR}/${CONFIG}/${TARGET}.exe" DESTINATION "${OUTPUT_DIR}")
endif()

if(EXISTS "${CMAKE_CURRENT_BINARY_DIR}/${CONFIG}/${TARGET}.dll")
    message(STATUS "${CMAKE_CURRENT_BINARY_DIR}/${CONFIG}/${TARGET}.dll" -> "${OUTPUT_DIR}")
    file(COPY "${CMAKE_CURRENT_BINARY_DIR}/${CONFIG}/${TARGET}.dll" DESTINATION "${OUTPUT_DIR}")
endif()

if(EXISTS "${CMAKE_CURRENT_BINARY_DIR}/${CONFIG}/${TARGET}.pdb")
    message(STATUS "${CMAKE_CURRENT_BINARY_DIR}/${CONFIG}/${TARGET}.pdb" -> "${OUTPUT_DIR}")
    file(COPY "${CMAKE_CURRENT_BINARY_DIR}/${CONFIG}/${TARGET}.pdb" DESTINATION "${OUTPUT_DIR}")
endif()

if(EXISTS "${SRC_DIR}/data")
    file(GLOB data_files "${SRC_DIR}/data/*")
    foreach(data_file ${data_files})
        message(STATUS "${data_file} -> ${DATA_OUTPUT_DIR}")
        file(COPY "${data_file}" DESTINATION "${DATA_OUTPUT_DIR}")
    endforeach()
endif()

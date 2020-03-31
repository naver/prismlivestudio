add_library(log STATIC log.cpp log.h)
add_dependencies(log liblog)
target_link_libraries(log liblog)

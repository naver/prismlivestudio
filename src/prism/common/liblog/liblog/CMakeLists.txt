add_definitions(-DLIBLOG_LIB -D_CRT_SECURE_NO_WARNINGS)
add_library(liblog SHARED liblog.cpp liblog.h)
add_obs_dependency(liblog)
link_obs(liblog)

install_obs_core(liblog)
copy_prism_target(liblog)


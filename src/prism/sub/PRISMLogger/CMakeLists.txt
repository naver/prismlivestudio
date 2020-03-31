project(PRISMLogger)

message(STATUS "Sub: PRISMLogger")

add_definitions(-D_CRT_SECURE_NO_WARNINGS)

add_executable(PRISMLogger main.cpp)
link_nelo(PRISMLogger)

install_obs_core(PRISMLogger)
copy_prism_target(PRISMLogger)


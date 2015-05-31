include_directories (container)
include_directories (container_container)

add_subdirectory (container)
add_subdirectory (container_container)

set (SRCS ${SRCS} mode_default)
set (SRCS ${SRCS} window_manager_default)
set (SRCS ${SRCS} mode_3panel)
set (SRCS ${SRCS} window_manager_3panel)

add_library (tiling_modes ${SRCS})

target_link_libraries (tiling_modes container_container container frontend)

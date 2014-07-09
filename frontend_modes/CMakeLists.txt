set (SRCS ${SRCS} mode_simple)
set (SRCS ${SRCS} mode_default)
#set (SRCS ${SRCS} mode_3panel)
set (SRCS ${SRCS} window_manager_default)
#set (SRCS ${SRCS} window_manager_3panel)

include_directories (${PROJECT_SOURCE_DIR}/frontend)
include_directories (${PROJECT_SOURCE_DIR}/frontend_container)
include_directories (${PROJECT_SOURCE_DIR}/frontend_container_container)

add_library (frontend_modes ${SRCS})

target_link_libraries (frontend_modes frontend_container_container frontend_container)

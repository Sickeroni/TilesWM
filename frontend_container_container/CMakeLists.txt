set (SRCS ${SRCS} container_util)
set (SRCS ${SRCS} container_container)
set (SRCS ${SRCS} container_container_layout)
set (SRCS ${SRCS} theme)

include_directories (${PROJECT_SOURCE_DIR}/frontend)
include_directories (${PROJECT_SOURCE_DIR}/frontend_container)

add_library (frontend_container_container ${SRCS})

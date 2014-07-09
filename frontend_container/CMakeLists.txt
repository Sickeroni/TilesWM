set (SRCS ${SRCS} container)
set (SRCS ${SRCS} client_container)
set (SRCS ${SRCS} client_container_layout)
set (SRCS ${SRCS} theme)
set (SRCS ${SRCS} mouse_handler)

add_library (frontend_container ${SRCS})

include_directories (${PROJECT_SOURCE_DIR}/frontend)

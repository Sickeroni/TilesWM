include_directories (${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory (tiling)
add_subdirectory (floating)

set (SRCS ${SRCS} monitor)
set (SRCS ${SRCS} widget)
set (SRCS ${SRCS} child_widget)
set (SRCS ${SRCS} workspace)
set (SRCS ${SRCS} client)
set (SRCS ${SRCS} action_set)
set (SRCS ${SRCS} action_run)
set (SRCS ${SRCS} application)
set (SRCS ${SRCS} key_grab_handler)
set (SRCS ${SRCS} key_binding_set)
set (SRCS ${SRCS} frontend_theme)
set (SRCS ${SRCS} common_actions)

add_library (frontend ${SRCS})

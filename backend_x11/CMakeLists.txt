# include_directories (${PROJECT_SOURCE_DIR}/backend_x11_common)
include_directories (graphics_system/common)

add_subdirectory (graphics_system/native)
add_subdirectory (graphics_system/cairo)

set (SRCS ${SRCS} x11_widget_backend)
set (SRCS ${SRCS} x11_widget)
set (SRCS ${SRCS} x11_server_widget)
set (SRCS ${SRCS} x11_client_widget)
set (SRCS ${SRCS} x11_client)
set (SRCS ${SRCS} x11_application)
set (SRCS ${SRCS} x11_global)
set (SRCS ${SRCS} x11_key_grab_set)
set (SRCS ${SRCS} default_config_values)

add_library (backend_x11 ${SRCS})

target_link_libraries (backend_x11 X11 Xcursor)

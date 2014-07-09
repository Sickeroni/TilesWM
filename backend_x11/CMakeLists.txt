set (SRCS ${SRCS} x11_widget_backend)
set (SRCS ${SRCS} x11_widget)
set (SRCS ${SRCS} x11_server_widget)
set (SRCS ${SRCS} x11_client_widget)
set (SRCS ${SRCS} x11_client)
set (SRCS ${SRCS} x11_application)
set (SRCS ${SRCS} x11_global)
set (SRCS ${SRCS} x11_key_grab_set)
set (SRCS ${SRCS} default_config_values)

include_directories (${PROJECT_SOURCE_DIR}/backend_x11_common)

add_library (backend_x11 ${SRCS})

target_link_libraries (backend_x11 X11 Xcursor)

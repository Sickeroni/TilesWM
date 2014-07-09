set (SRCS ${SRCS} x11_canvas)
set (SRCS ${SRCS} x11_icon)
set (SRCS ${SRCS} x11_graphics_system_x11)

include_directories (${PROJECT_SOURCE_DIR}/backend_x11_common)

add_library (backend_x11_native ${SRCS})

target_link_libraries (backend_x11_native X11 Xcursor)

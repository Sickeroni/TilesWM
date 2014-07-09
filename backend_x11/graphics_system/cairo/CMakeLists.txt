set (SRCS ${SRCS} x11_graphics_system_cairo)

include_directories (${PROJECT_SOURCE_DIR}/common/cairo)
include_directories (${CAIROMM_INCLUDE_DIRS})

add_library (backend_x11_cairo ${SRCS})

target_link_libraries (backend_x11_cairo common_cairo ${CAIROMM_LIBS} X11 Xcursor)

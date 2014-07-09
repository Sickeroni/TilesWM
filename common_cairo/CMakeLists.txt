set (SRCS ${SRCS} cairo_canvas)
set (SRCS ${SRCS} cairo_icon)

include_directories (${CAIROMM_INCLUDE_DIRS})

add_library (common_cairo ${SRCS})

target_link_libraries (common_cairo ${CAIROMM_LIBS})

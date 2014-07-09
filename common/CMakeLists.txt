add_subdirectory (cairo)

set (SRCS ${SRCS} config)
set (SRCS ${SRCS} string_util)
set (SRCS ${SRCS} theme)

add_library (common ${SRCS})

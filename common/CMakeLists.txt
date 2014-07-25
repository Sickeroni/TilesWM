add_subdirectory (cairo)

set (SRCS ${SRCS} config)
set (SRCS ${SRCS} string_util)

add_library (common ${SRCS})

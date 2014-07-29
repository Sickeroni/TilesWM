set (SRCS ${SRCS} mode_simple)
set (SRCS ${SRCS} client_frame)
set (SRCS ${SRCS} theme)

add_library (floating_modes ${SRCS})

target_link_libraries (floating_modes frontend)

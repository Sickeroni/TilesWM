set (SRCS ${SRCS} mode_simple)

add_library (floating_modes ${SRCS})

target_link_libraries (floating_modes frontend)

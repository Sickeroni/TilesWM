add_executable (misc misc.cpp)
target_link_libraries (misc ${COMMON_LIBS})

add_custom_target (run-tests
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/misc
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Running Tests" VERBATIM)

add_dependencies (run-tests misc)

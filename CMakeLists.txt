# The name of our project is "PROJECT". CMakeLists files in this project can
# refer to the root source directory of the project as ${PROJECT_SOURCE_DIR} and
# to the root binary directory of the project as ${PROJECT_BINARY_DIR}.
cmake_minimum_required (VERSION 2.8.9)
project (PROJECT)

set (CMAKE_CXX_FLAGS "-std=c++11")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wfatal-errors")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wuninitialized")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Woverloaded-virtual")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-strict-aliasing")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fstrict-aliasing")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fpermissive")

set (CAIROMM_INCLUDE_DIRS ${CAIROMM_INCLUDE_DIRS} /usr/include/cairomm-1.0)
set (CAIROMM_INCLUDE_DIRS ${CAIROMM_INCLUDE_DIRS} /usr/include/sigc++-2.0)
set (CAIROMM_INCLUDE_DIRS ${CAIROMM_INCLUDE_DIRS} /usr/include/cairo)
set (CAIROMM_INCLUDE_DIRS ${CAIROMM_INCLUDE_DIRS} /usr/include/freetype2)
# workaround debian bug
set (CAIROMM_INCLUDE_DIRS ${CAIROMM_INCLUDE_DIRS} /usr/lib/x86_64-linux-gnu/sigc++-2.0/include)

set (CAIROMM_LIBS cairomm-1.0)

include_directories (common)

add_subdirectory (common)
add_subdirectory (frontend)
add_subdirectory (backend_x11)

include_directories (frontend)
include_directories (frontend/tiling)
include_directories (frontend/floating)

set (COMMON_LIBS floating_modes tiling_modes frontend common)

add_executable (ttmwm ttmwm.cpp)
target_link_libraries (ttmwm backend_x11 backend_x11_native ${COMMON_LIBS})

add_executable (ttmwm-cairo ttmwm.cpp)
target_link_libraries (ttmwm-cairo backend_x11 backend_x11_cairo ${COMMON_LIBS})

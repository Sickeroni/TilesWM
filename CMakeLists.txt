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
add_subdirectory (common_cairo)
add_subdirectory (frontend)
add_subdirectory (frontend_container)
add_subdirectory (frontend_container_container)
add_subdirectory (frontend_modes)
add_subdirectory (backend_x11)
add_subdirectory (backend_x11_native)
add_subdirectory (backend_x11_cairo)

link_directories (common)

include_directories (frontend)
include_directories (frontend_modes)

add_executable (ttmwm ttmwm.cpp)
add_executable (ttmwm-cairo ttmwm.cpp)

target_link_libraries (ttmwm backend_x11 backend_x11_native frontend_modes frontend common)
target_link_libraries (ttmwm-cairo backend_x11 backend_x11_cairo frontend_modes frontend common)

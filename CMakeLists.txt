cmake_minimum_required (VERSION 2.6)
project (Libparser)

set (CMAKE_CXX_FLAGS "-std=c++14 -Wall -Werror")

include_directories ("${PROJECT_SOURCE_DIR}/test")
add_subdirectory (src)
add_subdirectory (test)
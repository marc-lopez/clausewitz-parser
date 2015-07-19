cmake_minimum_required (VERSION 2.6)
project (Libparser)

option(ADD_COVERAGE_INFO "Add coverage information" OFF)

set (CMAKE_CXX_FLAGS "-std=c++14 -Wall -Werror")

if (ADD_COVERAGE_INFO)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage -g -O0")
endif()

include_directories ("${PROJECT_SOURCE_DIR}/test")
add_subdirectory (src)
add_subdirectory (test)
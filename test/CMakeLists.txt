# We need thread support
find_package(Threads REQUIRED)

# Enable ExternalProject CMake module
include(ExternalProject)

if (WIN32)
    set(DISABLE_PTHREADS 1)
else()
    set(DISABLE_PTHREADS 0)
endif()


# Download and install GoogleMock
ExternalProject_Add(
    gmock
    URL https://googlemock.googlecode.com/files/gmock-1.7.0.zip
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}/gmock
    # Disable install step
    INSTALL_COMMAND ""
    CMAKE_ARGS -Dgtest_disable_pthreads=${DISABLE_PTHREADS}
)

# Create a libgmock target to be used as a dependency by test programs
add_library(libgmock IMPORTED STATIC GLOBAL)
add_dependencies(libgmock gmock)

# Set gmock properties
ExternalProject_Get_Property(gmock source_dir binary_dir)
set_target_properties(libgmock PROPERTIES
    "IMPORTED_LOCATION" "${binary_dir}/libgmock.a"
    "IMPORTED_LINK_INTERFACE_LIBRARIES" "${CMAKE_THREAD_LIBS_INIT}"
)
include_directories("${source_dir}/include")
include_directories("${source_dir}/gtest/include")

include_directories("${CMAKE_SOURCE_DIR}/src")

file(GLOB TESTS *.cpp)
add_executable(libparser_tests ${TESTS})

target_link_libraries(
    libparser_tests
    libparser
    libgmock
)
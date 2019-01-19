
cmake_minimum_required(VERSION 3.9)

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

project(dummy-player
    VERSION 1.0
    DESCRIPTION "A C++ pet project"
    LANGUAGES CXX
)

enable_testing()

# Only require C++14 for thirdparties
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


add_subdirectory(${CMAKE_SOURCE_DIR}/thirdparties)
add_subdirectory(${CMAKE_SOURCE_DIR}/lib)
add_subdirectory(${CMAKE_SOURCE_DIR}/app)

add_subdirectory(${CMAKE_SOURCE_DIR}/tests)

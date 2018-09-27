cmake_minimum_required(VERSION 3.8)

# add this options before PROJECT keyword
set(CMAKE_DISABLE_SOURCE_CHANGES ON)
set(CMAKE_DISABLE_IN_SOURCE_BUILD ON)

project(mpi-action-framework)

find_package(MPI REQUIRED)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/src)

enable_testing()
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/examples)
# add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/tests)


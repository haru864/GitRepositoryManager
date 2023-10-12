cmake_minimum_required(VERSION 3.13)

project(GitRepositoryManager CXX)

set(LIBGIT2_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/include")
set(LIBGIT2_LIB_DIR "${CMAKE_SOURCE_DIR}/libs")
set(SRC_DIR "${CMAKE_SOURCE_DIR}/src")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/build")

include_directories(${LIBGIT2_INCLUDE_DIR})

add_executable(grm "${SRC_DIR}/grm.cpp" "${SRC_DIR}/common.cpp" "${SRC_DIR}/functions.cpp")
target_link_libraries(grm "${LIBGIT2_LIB_DIR}/libgit2.so.1.7")

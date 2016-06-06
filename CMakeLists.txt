cmake_minimum_required (VERSION 2.8)
project (clasp_w)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D WITH_THREADS=0")
include_directories(libclasp)

add_subdirectory(libclasp)
add_subdirectory(clasp_w)

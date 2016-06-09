cmake_minimum_required (VERSION 2.8)
project (clasp_w)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -D WITH_THREADS=0")
include_directories(libclasp)

if(WIN32)
  include_directories(${CMAKE_CURRENT_SOURCE_DIR}/third_party/pthread/include)
  if(MSVC14)
    add_definitions(-DHAVE_STRUCT_TIMESPEC)
  endif(MSVC14)
endif(WIN32)

add_subdirectory(libclasp)
add_subdirectory(clasp_w)

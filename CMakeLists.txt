cmake_minimum_required (VERSION 3.1)
project (clasp_w)

set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY
  ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY
  ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY
  ${CMAKE_BINARY_DIR}/bin)

add_definitions(-DWITH_THREADS=0)
include_directories(libclasp)

if(WIN32)
  include_directories(${CMAKE_CURRENT_SOURCE_DIR}/third_party/pthread/include)
  if(MSVC14)
    add_definitions(-DHAVE_STRUCT_TIMESPEC)
  endif(MSVC14)
endif(WIN32)

add_subdirectory(libclasp)
add_subdirectory(clasp_w)

file(
  GLOB_RECURSE
  SOURCE_FILES
  *.cpp
  )

add_executable(clasp_w
  ${SOURCE_FILES})

target_link_libraries(clasp_w
  clasp pthread)

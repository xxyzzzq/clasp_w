file(
  GLOB_RECURSE
  SOURCE_FILES
  src/*.cpp
  )

add_library(clasp
  ${SOURCE_FILES})

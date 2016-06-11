file(
  GLOB_RECURSE
  SOURCE_FILES
  *.cpp
  )

set(PTHREAD_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../third_party/pthread/)
if(WIN32)
  if(CMAKE_CL_64)
    link_directories(${PTHREAD_DIR}/lib/x64)
    add_custom_command(TARGET clasp_w
      PRE_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy
        ${PTHREAD_DIR}/lib/x64/pthreadVC2.dll
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
  else(CMAKE_CL_64)
    link_directories(${PTHREAD_DIR}/lib/x86)
    add_custom_command(TARGET clasp_w
      PRE_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy
        ${PTHREAD_DIR}/lib/x86/pthreadVC2.dll
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
  endif(CMAKE_CL_64)
  set(PTHREAD_LIBRARY pthreadVC2.lib)
else(WIN32)
  set(PTHREAD_LIBRARY pthread)
endif(WIN32)

add_executable(clasp_w
  ${SOURCE_FILES})

  
target_link_libraries(clasp_w
  clasp ${PTHREAD_LIBRARY})

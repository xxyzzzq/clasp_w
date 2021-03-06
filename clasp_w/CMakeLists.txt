file(
  GLOB_RECURSE
  SOURCE_FILES
  *.cpp
  )

set(PTHREAD_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../third_party/pthread)
if(WIN32)
  if(CMAKE_CL_64)
    link_directories(${PTHREAD_DIR}/lib/x64)
  else(CMAKE_CL_64)
    link_directories(${PTHREAD_DIR}/lib/x86)
  endif(CMAKE_CL_64)
  set(PTHREAD_LIBRARY pthreadVC2.lib)
else(WIN32)
  set(PTHREAD_LIBRARY pthread)
endif(WIN32)

add_executable(clasp_w
  ${SOURCE_FILES})

target_link_libraries(clasp_w
  clasp ${PTHREAD_LIBRARY})

if(WIN32)
  get_target_property(BINARY_DIR clasp_w RUNTIME_OUTPUT_DIRECTORY)
  message(${BINARY_DIR})
  if(CMAKE_CL_64)
    add_custom_command(TARGET clasp_w
      PRE_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy
        ${PTHREAD_DIR}/dll/x64/pthreadVC2.dll
        ${BINARY_DIR}/${CMAKE_CFG_INTDIR})
  else(CMAKE_CL_64)
    add_custom_command(TARGET clasp_w
      PRE_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy
        ${PTHREAD_DIR}/dll/x86/pthreadVC2.dll
        ${BINARY_DIR}/${CMAKE_CFG_INTDIR})
  endif(CMAKE_CL_64)
endif(WIN32)

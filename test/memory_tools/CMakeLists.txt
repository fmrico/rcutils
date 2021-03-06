# Create the memory_tools library which is used by the tests.
add_library(${PROJECT_NAME}_memory_tools SHARED memory_tools.cpp)
if(NOT WIN32)
  # otherwise the compiler will error since the allocator functions have
  # different exception specifiers
  set_target_properties(${PROJECT_NAME}_memory_tools
    PROPERTIES COMPILE_FLAGS "-Wno-pedantic")
endif()
if(APPLE)
  # Create an OS X specific version of the memory tools that does interposing.
  # See: http://toves.freeshell.org/interpose/
  add_library(${PROJECT_NAME}_memory_tools_interpose SHARED memory_tools_osx_interpose.cpp)
  target_link_libraries(${PROJECT_NAME}_memory_tools_interpose ${GMOCK_LIBRARIES})
  list(APPEND extra_test_libraries ${PROJECT_NAME}_memory_tools_interpose)
  list(APPEND extra_test_env
    DYLD_INSERT_LIBRARIES=$<TARGET_FILE:${PROJECT_NAME}_memory_tools_interpose>)
elseif(UNIX)
  # On Linux like systems, add dl and use the normal library and DL_PRELOAD.
  list(APPEND extra_test_libraries dl)
  list(APPEND extra_test_env DL_PRELOAD=$<TARGET_FILE:${PROJECT_NAME}_memory_tools>)
endif()
list(APPEND extra_lib_dirs $<TARGET_FILE_DIR:${PROJECT_NAME}_memory_tools>)
target_link_libraries(${PROJECT_NAME}_memory_tools ${extra_test_libraries})
target_compile_definitions(${PROJECT_NAME}_memory_tools
  PRIVATE "RCUTILS_MEMORY_TOOLS_BUILDING_DLL")
list(APPEND extra_test_libraries ${PROJECT_NAME}_memory_tools)

# Create tests for the memory tools library.
set(SKIP_TEST "")
if(WIN32)  # (memory tools doesn't do anything on Windows)
  set(SKIP_TEST "SKIP_TEST")
endif()

ament_add_gmock(test_memory_tools test_memory_tools.cpp
  ENV ${extra_test_env}
  ${SKIP_TEST}
)
if(TARGET test_memory_tools)
  target_link_libraries(test_memory_tools ${extra_test_libraries})
endif()

set(extra_test_libraries ${extra_test_libraries} PARENT_SCOPE)
set(extra_test_env ${extra_test_env} PARENT_SCOPE)
set(extra_lib_dirs ${extra_lib_dirs} PARENT_SCOPE)

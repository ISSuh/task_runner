CMAKE_MINIMUM_REQUIRED (VERSION 3.5)
PROJECT(task_runner)

#-------------------------------------------------------------------------------
# Set Compiler
#-------------------------------------------------------------------------------
SET(CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_MAKE_PROGRAM="make -j 4")
ADD_COMPILE_OPTIONS (-g -Wall -Wextra)

#-------------------------------------------------------------------------------
# Set Configure
#-------------------------------------------------------------------------------
SET(TASK_RUNNER_LIBRARY_NAME ${PROJECT_NAME})

#-------------------------------------------------------------------------------
# Set Build Variable
#-------------------------------------------------------------------------------
SET(PROJECT_DIR ${PROJECT_SOURCE_DIR})
SET(BUILD_DIR_EXAMPLES ${CMAKE_SOURCE_DIR}/build/example)
SET(BUILD_DIR_TESTS ${CMAKE_SOURCE_DIR}/build/tests)

include_directories(
  ${PROJECT_SOURCE_DIR}/src
)

SET(PROJECT_BASE_SRC
  ${PROJECT_DIR}/src/base/logging.cc
)

SET(PROJECT_CALLBACK_SRC
  # ${PROJECT_DIR}/src/callback/
)

SET(PROJECT_TASK_SRC
  ${PROJECT_DIR}/src/task/task_runner.cc
  ${PROJECT_DIR}/src/task/sequenced_task_runner.cc
)

SET(PROJECT_THREAD_SRC
  ${PROJECT_DIR}/src/thread/worker_thread.cc
)

SET(PROJECT_SRC
  src/task_manager.cc
  ${PROJECT_BASE_SRC}
  ${PROJECT_CALLBACK_SRC}
  ${PROJECT_TASK_SRC}
  ${PROJECT_THREAD_SRC}
)

#-------------------------------------------------------------------------------
# Set SubDirectory
#-------------------------------------------------------------------------------
add_library(${TASK_RUNNER_LIBRARY_NAME} STATIC ${PROJECT_SRC})
target_link_libraries(${TASK_RUNNER_LIBRARY_NAME}
 PUBLIC
  pthread
)

add_subdirectory(example)

#-------------------------------------------------------------------------------
# Set gTest
#-------------------------------------------------------------------------------
# include(cmake/googletest.cmake)

# fetch_googletest(
#     ${PROJECT_SOURCE_DIR}/cmake
#     ${PROJECT_BINARY_DIR}/googletest
# )

# enable_testing()

# add_subdirectory(tests)
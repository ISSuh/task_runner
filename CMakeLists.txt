CMAKE_MINIMUM_REQUIRED (VERSION 3.5)
PROJECT (TaskRunner)

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
SET(TARGET_NAME ${PROJECT_NAME})

#-------------------------------------------------------------------------------
# Set Build Variable
#-------------------------------------------------------------------------------
SET(PROJECT_DIR ${PROJECT_SOURCE_DIR})
SET(BUILD_DIR_EXAMPLES ${CMAKE_SOURCE_DIR}/build)
SET(BUILD_DIR_TESTS ${CMAKE_SOURCE_DIR}/build/bin/tests)

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
  ${PROJECT_DIR}/src/task/sequenced_task_runner.cc
)

SET(PROJECT_THREAD_SRC
  ${PROJECT_DIR}/src/thread/worker_thread.cc
)

SET(PROJECT_SRC
  ${PROJECT_BASE_SRC}
  ${PROJECT_CALLBACK_SRC}
  ${PROJECT_TASK_SRC}
  ${PROJECT_THREAD_SRC}
)

#-------------------------------------------------------------------------------
# Set SubDirectory
#-------------------------------------------------------------------------------
add_library(task_runner STATIC ${PROJECT_SRC})


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
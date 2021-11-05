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

SET(PROJECT_INCLUDES
    ${PROJECT_DIR}/src
)

#-------------------------------------------------------------------------------
# Set SubDirectory
#-------------------------------------------------------------------------------
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
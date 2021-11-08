project(Example)

message("TaskRunner Examples")

#-------------------------------------------------------------------------------
# Set Include Dir
#-------------------------------------------------------------------------------
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${BUILD_DIR_EXAMPLES} )

set(EXAMPLE_SRC
  example
)

#-------------------------------------------------------------------------------
# Final target
#-------------------------------------------------------------------------------
foreach(EXAMPLE_NAME ${EXAMPLE_SRC})
  add_executable( ${EXAMPLE_NAME} ${EXAMPLE_NAME}.cc )
  target_include_directories( ${EXAMPLE_NAME}
   PUBLIC
    ${PROJECT_INCLUDES}
  )

  target_link_libraries( ${EXAMPLE_NAME}
   PUBLIC
    ${TASK_RUNNER_LIBRARY_NAME}
  )

  set_property(TARGET ${EXAMPLE_NAME} PROPERTY FOLDER "example")
endforeach()

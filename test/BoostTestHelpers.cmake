function(add_boost_test SOURCE_FILE_NAME DEPENDENCY_LIB)
  get_filename_component(TEST_EXECUTABLE_NAME ${SOURCE_FILE_NAME} NAME_WE)

  add_executable(${TEST_EXECUTABLE_NAME} ${SOURCE_FILE_NAME})

  target_link_libraries(${TEST_EXECUTABLE_NAME} ${DEPENDENCY_LIB} ${Boost_LIBRARIES})

  file(READ "${SOURCE_FILE_NAME}" SOURCE_FILE_CONTENTS)

  # XXX this should not find tests who are commented out!!!
  string(REGEX MATCHALL "BOOST_AUTO_TEST_CASE\\( *([A-Za-z_0-9]+) *\\)"
    FOUND_TESTS ${SOURCE_FILE_CONTENTS})

  foreach(HIT ${FOUND_TESTS})

    string(REGEX REPLACE ".*\\( *([A-Za-z_0-9]+) *\\).*" "\\1" TEST_NAME ${HIT})

    add_test(NAME "${TEST_EXECUTABLE_NAME}.${TEST_NAME}"
      WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/test
      COMMAND ${TEST_EXECUTABLE_NAME}
      --run_test=${TEST_NAME} --catch_system_error=yes)

  endforeach()

endfunction()

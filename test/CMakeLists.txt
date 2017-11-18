# find some dependencies
find_package(Boost 1.59 COMPONENTS unit_test_framework REQUIRED)

include_directories(${PROJECT_SOURCE_DIR}/src ${Boost_INCLUDE_DIRS})

add_definitions(-DBOOST_TEST_DYN_LINK)

file(GLOB TEST_SRCS *.cpp)

include(BoostTestHelpers.cmake)

foreach(test_src ${TEST_SRCS})

  add_boost_test("${test_src}" orbits_lib)

endforeach(test_src)

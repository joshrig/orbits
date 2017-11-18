cmake_minimum_required(VERSION 3.9)

project(orbits VERSION 1.0 DESCRIPTION "Satellite Orbit Simulator")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

set(CMAKE_CXX_STANDARD 11)

# main executable directory
add_subdirectory(src)

# testing
enable_testing()
add_subdirectory(test)

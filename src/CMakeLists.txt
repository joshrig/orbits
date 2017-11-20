configure_file("settings.h.in" "settings.h")

#set(CMAKE_VERBOSE_MAKEFILE TRUE)

# find some dependencies
find_package(Boost 1.59 REQUIRED)
find_package(GLEW REQUIRED)
find_package(GLFW REQUIRED)
find_package(GLM REQUIRED)
find_package(OPENGL REQUIRED)

# tell the compiler where to look for our include files
include_directories(
  ${PROJECT_SOURCE_DIR}
  ${Boost_INCLUDE_DIRS}
  ${GLEW_INCLUDE_DIRS}
  ${GLFW_INCLUDE_DIRS}
  ${OPENGL_INCLUDE_DIR}
  ${GLM_INCLUDE_DIRS}
  )

# build the compile database for emacs irony
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

# create a static library of all the support modules sans main
add_library(orbits_lib
  Earth.cpp
  Floor.cpp
  Triangle.cpp
  Wavefront.cpp
  glutil.cpp
  BMPLoader.cpp)

# add the main executable
add_executable(orbits main.cpp)

# but tell cmake that our target, orbits, requires orbits_lib and some other stuff
target_link_libraries(
  orbits
  orbits_lib
  ${GLFW_LIBRARIES}
  ${GLEW_LIBRARIES}
  ${OPENGL_LIBRARIES}
  )

# where to install everything
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set(CMAKE_INSTALL_PREFIX "${PROJECT_SOURCE_DIR}/dist" CACHE PATH "..." FORCE)
endif()
FILE(GLOB shader_files "${CMAKE_CURRENT_SOURCE_DIR}/shaders/*")
FILE(GLOB resource_files
  "${CMAKE_CURRENT_SOURCE_DIR}/resources/*.obj"
  "${CMAKE_CURRENT_SOURCE_DIR}/resources/*.bmp"
  )
INSTALL(FILES ${shader_files} ${resource_files} DESTINATION bin)
INSTALL(TARGETS orbits RUNTIME DESTINATION bin)


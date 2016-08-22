cmake_minimum_required(VERSION 2.8)

project(grid_surface)

set(CMAKE_CXX_COMPILER g++)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(sources
  src/camera.cpp
  src/main.cpp
  src/surface.cpp
)

set(headers
  include/camera.hpp
  include/surface.hpp
)

find_package(OpenGL REQUIRED)
find_package(GLUT REQUIRED)
find_package(OpenCV REQUIRED)

include_directories(
  ${CMAKE_SOURCE_DIR}
  ${OpenGL_INCLUDE_DIRS}
  ${GLUT_INCLUDE_DIRS}
  ${OpenCV_INCLUDE_DIRS}
)

add_executable(${CMAKE_PROJECT_NAME} ${sources} ${headers})
target_link_libraries(
  ${CMAKE_PROJECT_NAME}
  ${OPENGL_LIBRARIES}
  ${GLUT_LIBRARIES}
  ${OpenCV_LIBRARIES}
)

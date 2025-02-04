cmake_minimum_required(VERSION 3.23.0 FATAL_ERROR)
project(
  inp_visualizer
  VERSION 0.0.1
  LANGUAGES C CXX)

set(QT_DIR "C:\\Qt\\6.8.1\\msvc2022_64\\lib\\cmake\\Qt6")

option(BUILD_ASSISTANT_TOOLS "Build assistant tools" OFF)
option(BUILD_UNIT_TESTS "Build unit tests" OFF)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}/bin)

if(MSVC)
  add_compile_options(/utf-8)
endif()
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
include(cotire)
include(VTKInit)

add_subdirectory(source)
add_subdirectory(assets)
add_subdirectory(share)
add_subdirectory(depends)

if(BUILD_ASSISTANT_TOOLS)
  add_subdirectory(tools)
endif()

if(BUILD_UNIT_TESTS)
  add_subdirectory(tests)
endif()

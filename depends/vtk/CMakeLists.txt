cmake_minimum_required(VERSION 3.12)

set(LIBRARY_TARGET_NAME vtk)
project(${LIBRARY_TARGET_NAME})
add_library(${LIBRARY_TARGET_NAME} INTERFACE)
target_link_libraries(${LIBRARY_TARGET_NAME} INTERFACE ${VTK_LIBRARIES})
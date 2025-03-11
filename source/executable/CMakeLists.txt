# 可执行程序

cmake_minimum_required(VERSION 3.23.0 FATAL_ERROR)

set(Qt6_DIR "C:/Qt/6.5.39/win64")

set(TARGET_NAME ${PROJECT_NAME})

find_package(Qt6 REQUIRED COMPONENTS Core)
find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(Qt6 REQUIRED COMPONENTS Network)
find_package(Qt6 REQUIRED COMPONENTS Concurrent)
find_package(Qt6 REQUIRED COMPONENTS OpenGLWidgets)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_executable(
  ${TARGET_NAME}
  main.cpp
  mainwindow.h
  mainwindow.cpp
  settings.h
  settings.cpp
  runtime.h
  runtime.cpp)

target_link_libraries(
  ${TARGET_NAME}
  PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Concurrent
          assets utils widgets model)

set_target_properties(${TARGET_NAME} PROPERTIES DEBUG_POSTFIX
                                                "${CMAKE_DEBUG_POSTFIX}")

if(WIN32)
  if(CMAKE_BUILD_TYPE STREQUAL "Release")
    # set_target_properties(${TARGET_NAME} PROPERTIES WIN32_EXECUTABLE ON)
  endif()
endif()

add_subdirectory(model)
add_subdirectory(widgets)

add_dependencies(${TARGET_NAME} RuntimeResources)

include(GNUInstallDirs)
install(TARGETS ${TARGET_NAME} RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

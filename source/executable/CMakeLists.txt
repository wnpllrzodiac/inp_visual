# 可执行程序

cmake_minimum_required(VERSION 3.23.0 FATAL_ERROR)

set(TARGET_NAME ${PROJECT_NAME})

find_package(QT NAMES Qt6 Qt5 CONFIG REQUIRED COMPONENTS Widgets Network
                                                         Concurrent)
find_package(Qt${QT_VERSION_MAJOR} CONFIG REQUIRED COMPONENTS Widgets Network
                                                              Concurrent)

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
  ${TARGET_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Widgets
                         Qt${QT_VERSION_MAJOR}::Concurrent assets utils widgets)

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

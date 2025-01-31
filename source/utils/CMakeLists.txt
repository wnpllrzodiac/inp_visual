# 工具类
get_filename_component(TARGET_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

find_package(QT NAMES Qt6 Qt5 CONFIG REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} CONFIG REQUIRED COMPONENTS Core)

add_library(${TARGET_NAME} STATIC logging.h logging.cpp pathutil.h pathutil.cpp)

target_link_libraries(${TARGET_NAME} PUBLIC Qt${QT_VERSION_MAJOR}::Core
                                            spdlog::spdlog fmt::fmt)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  target_compile_definitions(${TARGET_NAME} PRIVATE "DEBUG")
endif()

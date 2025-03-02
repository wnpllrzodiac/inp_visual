# 程序运行配置相关文件

find_package(QT NAMES Qt6 Qt5 CONFIG REQUIRED COMPONENTS Core LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} CONFIG REQUIRED COMPONENTS Core
                                                              LinguistTools)

if(NOT CMAKE_RUNTIME_OUTPUT_DIRECTORY)
  message(FATAL_ERROR "Please set the CMAKE_RUNTIME_OUTPUT_DIRECTORY")
endif()

list(APPEND TRANSLATE_DIRECTORIES ${CMAKE_SOURCE_DIR}/source/)

add_custom_command(
  OUTPUT ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/translations
  COMMAND ${CMAKE_COMMAND} -E make_directory
          ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/translations)

add_custom_command(
  OUTPUT ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/config.ini
  COMMAND
    ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_CURRENT_SOURCE_DIR}/config.ini
    ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/config.ini)

set(LUPDATE_ARGS -silent -locations none -no-ui-lines)

add_custom_target(
  RuntimeResources
  DEPENDS ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/translations
  DEPENDS ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/config.ini
  COMMAND Qt${QT_VERSION_MAJOR}::lupdate ${LUPDATE_ARGS} -target-language zh_CN
          ${TRANSLATE_DIRECTORIES} -ts ${CMAKE_CURRENT_SOURCE_DIR}/zh_CN.ts
  COMMAND Qt${QT_VERSION_MAJOR}::lupdate ${LUPDATE_ARGS} -target-language en_US
          ${TRANSLATE_DIRECTORIES} -ts ${CMAKE_CURRENT_SOURCE_DIR}/en_US.ts
  COMMAND Qt${QT_VERSION_MAJOR}::lrelease ${CMAKE_CURRENT_SOURCE_DIR}/zh_CN.ts
          -qm ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/translations/zh_CN.qm
  COMMAND Qt${QT_VERSION_MAJOR}::lrelease ${CMAKE_CURRENT_SOURCE_DIR}/en_US.ts
          -qm ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/translations/en_US.qm)

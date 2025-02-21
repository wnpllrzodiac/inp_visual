# 程序运行时资产（字体，图标，主题等等）

set(TARGET_NAME assets)

find_package(QT NAMES Qt6 Qt5 CONFIG REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} CONFIG REQUIRED COMPONENTS Core)

set(CMAKE_AUTORCC ON)

add_library(${TARGET_NAME} STATIC assets.qrc)
target_link_libraries(${TARGET_NAME} PUBLIC Qt${QT_VERSION_MAJOR}::Core)

set(ASSET_FILES styles/main.qss)

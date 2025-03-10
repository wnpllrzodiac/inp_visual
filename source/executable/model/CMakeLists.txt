set(TARGET_NAME model)

set(PRIVATE_SOURCES projectmodel.h projectmodel.cpp inpreader.h inpreader.cpp)

find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Gui)

add_library(${TARGET_NAME} STATIC ${PRIVATE_SOURCES})
create_and_init_vtk_target(${TARGET_NAME})

target_link_libraries(${TARGET_NAME}
    PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Gui utils
    vtk
)

target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

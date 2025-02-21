set(TARGET_NAME model)

set(PRIVATE_SOURCES projectmodel.h projectmodel.cpp inpreader.h inpreader.cpp)

add_library(${TARGET_NAME} STATIC ${PRIVATE_SOURCES})
create_and_init_vtk_target(${TARGET_NAME})

target_link_libraries(${TARGET_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Core utils
                                             vtk)

target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

set(TARGET_NAME model)

set(PRIVATE_SOURCES projectmodel.h projectmodel.cpp)
add_library(${TARGET_NAME} STATIC ${PRIVATE_SOURCES})

target_link_libraries(${TARGET_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Widgets
                                             Qt${QT_VERSION_MAJOR}::Core utils)

target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

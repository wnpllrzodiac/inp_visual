set(TARGET_NAME widgets)
set(SOURCES vtkwidget.cpp vtkwidget.h configwidget.cpp configwidget.h)

add_library(${TARGET_NAME} STATIC ${SOURCES})

create_and_init_vtk_target(${TARGET_NAME})
target_link_libraries(
  ${TARGET_NAME}
  PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Core utils model
  PUBLIC vtk)

target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(vtk)
add_subdirectory(config)

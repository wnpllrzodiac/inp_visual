set(TARGET_NAME widgets)
set(SOURCES vtkwidget.cpp vtkwidget.h configwidget.cpp configwidget.h
            inpreader.cpp inpreader.h)

add_library(${TARGET_NAME} STATIC ${SOURCES})

target_link_libraries(${TARGET_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Widgets
                                             Qt${QT_VERSION_MAJOR}::Core utils)
find_package(
  VTK
  COMPONENTS InteractionStyle RenderingOpenGL2 ViewsQt GUISupportQt
  REQUIRED)

target_link_libraries(${TARGET_NAME} PUBLIC VTK::GUISupportQt VTK::ViewsQt
                                            VTK::RenderingOpenGL2)
target_include_directories(${TARGET_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

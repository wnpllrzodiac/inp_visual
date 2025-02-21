set(PRIVATE_SOURCES
    unitwidget.cpp
    unitwidget.h
    controlparameterswidget.cpp
    controlparameterswidget.h
    elementpropertieswidget.cpp
    elementpropertieswidget.h
    jointpropertieswidget.cpp
    jointpropertieswidget.h
    dfnwidget.cpp
    dfnwidget.h
    boundaryconditionswidget.cpp
    boundaryconditionswidget.h
    inputdelegate.cpp
    inputdelegate.h)

target_sources(${TARGET_NAME} PRIVATE ${PRIVATE_SOURCES})

# inp_visual

## build vtk with VS2022 and QT6.8
1. git clone https://github.com/Kitware/VTK.git
2. git checkout v9.3.1
3. cmake check UI "Grouped", "Advanced"
4. "Add Entry": CMAKE_DEBUG_POSTFIX, type: STRING，value: -gd      (build debug/release both)
5. Configure
    + BUILD_SHARED_LIBS
    + CMAKE_INSTALL_PREFIX
    + VTK_GROUP_ENABLE_Qt           YES
    + VTK_GROUP_ENABLE_Rendering    YES
    + VTK_GROUP_ENABLE_StandAlone   WANT
    + VTK_GROUP_ENABLE_Views        YES
    + Qt6_DIR                       QT\6.5.39\win64\lib\cmake\Qt6
    + CMAKE_INSTALL_PREFIX          sdk/VTK
6. Generate
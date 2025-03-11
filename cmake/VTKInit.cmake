# VTKInit.cmake
set(VTK_DIR "D:/git/inp_visual/sdk/VTK/lib/cmake/vtk-9.3")
# message(${VTK_DIR})
find_package(VTK QUIET)

if(NOT VTK_FOUND)
  message(FATAL_ERROR "failed to find vtk")
endif()

include_directories(${VTK_USE_FILE})

function(create_and_init_vtk_target target_name)
  # 初始化 VTK 模块
  vtk_module_autoinit(TARGETS ${target_name} MODULES ${VTK_LIBRARIES})
endfunction()

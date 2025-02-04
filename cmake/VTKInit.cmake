# VTKInit.cmake
set(VTK_DIR
    "F:\\kits\\VTK-9.4.1\\Install_lib\\lib\\cmake\\vtk-9.4")
# message(${VTK_DIR})
find_package(VTK QUIET)

if(NOT VTK_FOUND)
  # VTK 未在本地找到，从 Git 克隆
  message("VTK 未在本地找到，尝试从 Git 克隆...")
endif()

include_directories(${VTK_USE_FILE})

function(create_and_init_vtk_target target_name)
  # 初始化 VTK 模块
  vtk_module_autoinit(TARGETS ${target_name} MODULES ${VTK_LIBRARIES})
endfunction()

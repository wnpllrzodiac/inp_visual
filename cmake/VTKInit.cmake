# VTKInit.cmake
set(VTK_DIR
    "D:\\Users\\walt\\workspace\\kits\\vtk9.2\\install\\lib\\cmake\\vtk-9.2")
# message(${VTK_DIR})
find_package(VTK QUIET)

if(NOT VTK_FOUND)
  # VTK 未在本地找到，从 Git 克隆
  message("VTK 未在本地找到，尝试从 Git 克隆...")

  # 指定 Git 克隆的目录
  set(VTK_SOURCE_DIR "${CMAKE_BINARY_DIR}/VTK")

  # 从 Git 克隆 VTK
  execute_process(
    COMMAND git clone --branch v9.2.0 https://gitlab.kitware.com/vtk/vtk.git
            ${VTK_SOURCE_DIR})

  # 添加子目录
  add_subdirectory(${VTK_SOURCE_DIR})

  # 更新 VTK_DIR 以使用新构建的 VTK
  set(VTK_DIR "${VTK_SOURCE_DIR}/build")
endif()

include_directories(${VTK_USE_FILE})

function(create_and_init_vtk_target target_name)
  # 初始化 VTK 模块
  vtk_module_autoinit(TARGETS ${target_name} MODULES ${VTK_LIBRARIES})
endfunction()

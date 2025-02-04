add_subdirectory(fmt)

set(SPDLOG_FMT_EXTERNAL
    ON
    CACHE BOOL "" FORCE)
    
add_subdirectory(spdlog)
add_subdirectory(vtk)

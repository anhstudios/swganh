
configure_file (
  "${CMAKE_CURRENT_SOURCE_DIR}/version.h.in"
  "${CMAKE_CURRENT_BINARY_DIR}/version.h"
)

if(WIN32)
    include(Win32PostBuild)
else()
    include(PosixPostBuild)
endif()

## project executables
add_subdirectory(swganh)

## publish 14 core implementation
add_subdirectory(swganh_core)

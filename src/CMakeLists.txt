
configure_file (
  "${CMAKE_CURRENT_SOURCE_DIR}/version.h.in"
  "${CMAKE_CURRENT_BINARY_DIR}/version.h"
)

## project libraries
add_subdirectory(anh)

## project executables
add_subdirectory(swganh)
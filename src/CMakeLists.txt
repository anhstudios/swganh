
configure_file (
  "${CMAKE_CURRENT_SOURCE_DIR}/version.h.in"
  "${CMAKE_CURRENT_BINARY_DIR}/version.h"
)

# make sure the source directory and the binary directory (where any generated
# files will be found) are automatically searched for includes
include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

if(WIN32)
    include(Win32PostBuild)
endif()

## project libraries
add_subdirectory(anh)

## project plugins
add_subdirectory(plugins)

## project executables
add_subdirectory(swganh)
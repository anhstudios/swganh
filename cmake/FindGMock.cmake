find_path(GMOCK_INCLUDE_DIR gmock/gmock.h
    HINTS
        $ENV{GMock_ROOT}
    PATH_SUFFIXES include
    PATHS
        ${GMock_ROOT}
        ${GMock_INCLUDEDIR}
)

find_library(GMOCK_LIBRARY 
    NAMES gmock
    PATH_SUFFIXES lib Debug Release
    HINTS
        $ENV{GMock_ROOT}
        ${GMock_ROOT}
        ${GMock_LIBRARYDIR}
)

# handle the QUIETLY and REQUIRED arguments and set OPENAL_FOUND to TRUE if
# all listed variables are TRUE
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(GMock DEFAULT_MSG GMOCK_LIBRARY GMOCK_INCLUDE_DIR)

mark_as_advanced(GMOCK_LIBRARY GMOCK_INCLUDE_DIR)


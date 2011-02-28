find_path(ZLIB_INCLUDE_DIR zlib.h
    HINTS
        $ENV{ZLib_ROOT}
    PATH_SUFFIXES include
    PATHS
        ${ZLib_ROOT}
        ${ZLib_INCLUDEDIR}
)

find_library(ZLIB_LIBRARY 
    NAMES z zlib zlibd
    PATH_SUFFIXES lib Debug Release
    HINTS
        $ENV{ZLib_ROOT}
        ${ZLib_ROOT}
        ${ZLib_LIBRARYDIR}
)

# handle the QUIETLY and REQUIRED arguments and set OPENAL_FOUND to TRUE if
# all listed variables are TRUE
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(ZLib DEFAULT_MSG ZLIB_LIBRARY ZLIB_INCLUDE_DIR)

mark_as_advanced(ZLIB_LIBRARY ZLIB_INCLUDE_DIR)
